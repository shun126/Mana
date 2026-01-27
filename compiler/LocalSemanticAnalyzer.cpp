/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#include "LocalSemanticAnalyzer.h"
#include "ErrorHandler.h"
#include "NamespaceRegistry.h"
#include "StringPool.h"
#include "SyntaxNode.h"
#include <algorithm>
#include <string>
#include <vector>

namespace mana
{
	LocalSemanticAnalyzer::LocalSemanticAnalyzer(
		const std::shared_ptr<SymbolFactory>& symbolFactory,
		const std::shared_ptr<TypeDescriptorFactory>& typeDescriptorFactory,
		const std::shared_ptr<StringPool>& stringPool,
		const std::shared_ptr<NamespaceRegistry>& namespaceRegistry)
		: SemanticAnalyzer(symbolFactory, typeDescriptorFactory, stringPool)
		, mNamespaceRegistry(namespaceRegistry)
	{
		mUsingScopes.emplace_back();
	}

	void LocalSemanticAnalyzer::BeginNamespaceScope(const std::string_view& name)
	{
		EnterNamespace(name);
	}

	void LocalSemanticAnalyzer::EndNamespaceScope()
	{
		ExitNamespace();
	}

	std::string_view LocalSemanticAnalyzer::JoinQualifiedName(const std::string_view& left, const std::string_view& right) const
	{
		if (left.empty())
			return right;

		std::string combined;
		combined.reserve(left.size() + 2 + right.size());
		combined.append(left);
		combined.append("::");
		combined.append(right);
		return GetStringPool()->Set(combined);
	}

	std::string_view LocalSemanticAnalyzer::GetCurrentNamespace() const
	{
		if (mNamespaceStack.empty())
			return {};
		return mNamespaceStack.back();
	}

	bool LocalSemanticAnalyzer::IsQualifiedName(const std::string_view& name) const
	{
		return name.find("::") != std::string_view::npos;
	}

	std::string_view LocalSemanticAnalyzer::GetLastSegment(const std::string_view& name) const
	{
		const size_t delimiter = name.rfind("::");
		if (delimiter == std::string_view::npos)
			return name;
		return name.substr(delimiter + 2);
	}

	bool LocalSemanticAnalyzer::IsActorSymbol(const std::string_view& name) const
	{
		const std::shared_ptr<Symbol>& symbol = GetSymbolFactory()->Lookup(name);
		if (!symbol || symbol->GetClassTypeId() != Symbol::ClassTypeId::Type)
			return false;

		const std::shared_ptr<TypeDescriptor>& type = symbol->GetTypeDescriptor();
		return type && (type->GetId() == TypeDescriptor::Id::Actor || type->GetId() == TypeDescriptor::Id::Module);
	}

	bool LocalSemanticAnalyzer::IsTypeSymbol(const std::string_view& name) const
	{
		const std::shared_ptr<Symbol>& symbol = GetSymbolFactory()->Lookup(name);
		return symbol && symbol->GetClassTypeId() == Symbol::ClassTypeId::Type;
	}

	std::string_view LocalSemanticAnalyzer::ResolveAlias(const std::string_view& name) const
	{
		for (auto scopeIt = mUsingScopes.rbegin(); scopeIt != mUsingScopes.rend(); ++scopeIt)
		{
			const auto aliasIt = scopeIt->symbolAliases.find(name);
			if (aliasIt != scopeIt->symbolAliases.end())
				return aliasIt->second;
		}
		return {};
	}

	std::string_view LocalSemanticAnalyzer::ResolveTypeName(const std::string_view& name) const
	{
		if (IsQualifiedName(name))
			return name;

		const std::string_view currentNamespace = GetCurrentNamespace();
		if (!currentNamespace.empty())
		{
			const std::string_view qualified = JoinQualifiedName(currentNamespace, name);
			if (IsTypeSymbol(qualified))
				return qualified;
		}

		if (IsTypeSymbol(name))
			return name;

		std::vector<std::string_view> candidates;
		auto addCandidate = [&candidates](const std::string_view candidate)
		{
			if (candidate.empty())
				return;
			if (std::find(candidates.begin(), candidates.end(), candidate) == candidates.end())
				candidates.push_back(candidate);
		};

		for (auto scopeIt = mUsingScopes.rbegin(); scopeIt != mUsingScopes.rend(); ++scopeIt)
		{
			for (const std::string_view& nsPath : scopeIt->namespacePaths)
			{
				const std::string_view qualified = JoinQualifiedName(nsPath, name);
				if (IsTypeSymbol(qualified))
					addCandidate(qualified);
			}
		}

		if (!candidates.empty())
		{
			if (candidates.size() > 1)
			{
				std::string message = "ambiguous type reference '" + std::string(name) + "': ";
				for (size_t index = 0; index < candidates.size(); ++index)
				{
					if (index > 0)
						message += ", ";
					message += std::string(candidates[index]);
				}
				CompileError(message);
				return name;
			}
			return candidates.front();
		}

		return name;
	}

	void LocalSemanticAnalyzer::ResolveTypeDescriptionScoped(const std::shared_ptr<SyntaxNode>& node)
	{
		MANA_ASSERT(node);

		const std::string_view resolvedName = ResolveTypeName(node->GetString());
		if (resolvedName != node->GetString())
			node->Set(resolvedName);

		SemanticAnalyzer::ResolveTypeDescription(node);
		RejectActorTypeName(node);
	}

	void LocalSemanticAnalyzer::RejectActorTypeName(const std::shared_ptr<SyntaxNode>& node)
	{
		MANA_ASSERT(node);
		const std::shared_ptr<TypeDescriptor>& type = node->GetTypeDescriptor();
		if (!type || type->GetId() != TypeDescriptor::Id::Actor)
			return;

		if (type == GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Actor))
			return;

		CompileError({ "actor name '", type->GetName(), "' cannot be used as a type. Use 'actor'." });
		node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Actor));
	}

	void LocalSemanticAnalyzer::ResolveVariableDescription(const std::shared_ptr<SyntaxNode>& node, const Symbol::MemoryTypeId memoryTypeId, const bool isStaticVariable)
	{
		MANA_ASSERT(node);
		MANA_ASSERT(node->GetLeftNode() && node->GetLeftNode()->Is(SyntaxNode::Id::TypeDescription));
		ResolveTypeDescriptionScoped(node->GetLeftNode());

		MANA_ASSERT(node->GetRightNode() && node->GetRightNode()->Is(SyntaxNode::Id::Declarator));
		SemanticAnalyzer::ResolveDeclarator(node->GetRightNode(), isStaticVariable);

		GetSymbolFactory()->AllocateMemory(node->GetRightNode()->GetSymbol(), node->GetLeftNode()->GetTypeDescriptor(), memoryTypeId);
	}

	void LocalSemanticAnalyzer::EnterNamespace(const std::string_view& name)
	{
		const std::string_view fullName = JoinQualifiedName(GetCurrentNamespace(), name);
		mNamespaceStack.push_back(fullName);
		mUsingScopes.emplace_back();
	}

	void LocalSemanticAnalyzer::ExitNamespace()
	{
		if (!mNamespaceStack.empty())
			mNamespaceStack.pop_back();
		if (!mUsingScopes.empty())
			mUsingScopes.pop_back();
	}

	void LocalSemanticAnalyzer::ResolveUsingDeclaration(const std::shared_ptr<SyntaxNode>& node)
	{
		const std::string_view name = node->GetString();
		const std::string_view candidateName = IsQualifiedName(name)
			? name
			: JoinQualifiedName(GetCurrentNamespace(), name);

		const bool isNamespace = mNamespaceRegistry && mNamespaceRegistry->IsNamespace(candidateName);
		const bool isSymbol = IsActorSymbol(candidateName);

		if (isNamespace && isSymbol)
		{
			CompileError({ "ambiguous using '", name, "'" });
			return;
		}

		if (isNamespace)
		{
			if (!mUsingScopes.empty())
				mUsingScopes.back().namespacePaths.push_back(candidateName);
			return;
		}

		if (isSymbol)
		{
			const std::string_view alias = GetLastSegment(name);
			if (mUsingScopes.empty())
				return;

			auto& aliases = mUsingScopes.back().symbolAliases;
			if (aliases.find(alias) != aliases.end())
			{
				CompileError({ "duplicate using alias '", alias, "'" });
				return;
			}
			aliases.emplace(alias, candidateName);
			return;
		}

		CompileError({ "unresolved using '", name, "'" });
	}

	void LocalSemanticAnalyzer::ResolveActionReference(const std::shared_ptr<SyntaxNode>& node, const std::string_view& actionName)
	{
		const std::shared_ptr<SyntaxNode>& actorNode = node->GetRightNode();
		if (!actorNode)
		{
			CompileError("illegal action reference");
			return;
		}

		if (actorNode->Is(SyntaxNode::Id::Identifier))
		{
			const std::string_view rawName = actorNode->GetString();
			std::vector<std::string_view> candidates;

			auto addCandidate = [&candidates](const std::string_view candidate)
			{
				if (candidate.empty())
					return;
				if (std::find(candidates.begin(), candidates.end(), candidate) == candidates.end())
					candidates.push_back(candidate);
			};

			if (IsQualifiedName(rawName))
			{
				if (IsActorSymbol(rawName))
					addCandidate(rawName);
			}
			else
			{
				const std::string_view currentNamespace = GetCurrentNamespace();
				if (!currentNamespace.empty())
				{
					const std::string_view qualified = JoinQualifiedName(currentNamespace, rawName);
					if (IsActorSymbol(qualified))
						addCandidate(qualified);
				}

				if (IsActorSymbol(rawName))
					addCandidate(rawName);

				const std::string_view alias = ResolveAlias(rawName);
				if (!alias.empty() && IsActorSymbol(alias))
					addCandidate(alias);

				for (auto scopeIt = mUsingScopes.rbegin(); scopeIt != mUsingScopes.rend(); ++scopeIt)
				{
					for (const std::string_view& nsPath : scopeIt->namespacePaths)
					{
						const std::string_view qualified = JoinQualifiedName(nsPath, rawName);
						if (IsActorSymbol(qualified))
							addCandidate(qualified);
					}
				}
			}

			if (!candidates.empty())
			{
				if (candidates.size() > 1)
				{
					std::string message = "ambiguous actor reference '" + std::string(rawName) + "': ";
					for (size_t index = 0; index < candidates.size(); ++index)
					{
						if (index > 0)
							message += ", ";
						message += std::string(candidates[index]);
					}
					CompileError(message);
					return;
				}

				const std::string_view resolvedName = candidates.front();
				actorNode->Set(resolvedName);
				PostResolverResolve(actorNode);

				const std::shared_ptr<Symbol>& actorSymbol = actorNode->GetSymbol();
				if (!actorSymbol || actorSymbol->GetClassTypeId() != Symbol::ClassTypeId::Type)
				{
					CompileError({ "invalid actor reference '", resolvedName, "'" });
					return;
				}

				if (!HasAction(actorSymbol, actionName))
				{
					CompileError({ "action not found '", actionName, "' on actor '", resolvedName, "'" });
				}
				return;
			}
		}

		PostResolverResolve(actorNode);

		const std::shared_ptr<TypeDescriptor>& actorType = actorNode->GetTypeDescriptor();
		if (!actorType)
		{
			CompileError("incompatible type of operand");
			return;
		}

		if (actorType->GetId() == TypeDescriptor::Id::Actor)
		{
			const std::string_view typeName = actorType->GetName();
			if (!typeName.empty() && typeName != "actor")
			{
				if (const std::shared_ptr<Symbol>& typeSymbol = GetSymbolFactory()->Lookup(typeName))
				{
					if (typeSymbol->GetClassTypeId() == Symbol::ClassTypeId::Type && !HasAction(typeSymbol, actionName))
					{
						CompileError({ "action not found '", actionName, "' on actor '", typeName, "'" });
						return;
					}
				}
			}
			return;
		}

		if (actorType->GetId() == TypeDescriptor::Id::Reference && actorType->GetName() == "string")
			return;

		CompileError("incompatible type of operand");
	}

	bool LocalSemanticAnalyzer::HasAction(const std::shared_ptr<Symbol>& actorSymbol, const std::string_view& actionName)
	{
		if (!actorSymbol)
			return false;

		const std::shared_ptr<TypeDescriptor>& type = actorSymbol->GetTypeDescriptor();
		if (!type)
			return false;

		for (std::shared_ptr<Symbol> member = type->GetSymbolEntry(); member; member = member->GetNext())
		{
			if (member->GetClassTypeId() == Symbol::ClassTypeId::MemberFunction && member->GetName() == actionName)
				return true;
		}
		return false;
	}

	bool LocalSemanticAnalyzer::GetNodeType(TypeDescriptor::Id* t1, TypeDescriptor::Id* t2, const std::shared_ptr<const SyntaxNode>& node)
	{
		MANA_ASSERT(node);
		MANA_ASSERT(t1);
		MANA_ASSERT(t2);

		if (node->GetLeftNode() == nullptr || node->GetLeftNode()->GetTypeDescriptor() == nullptr)
		{
			CompileError("illegal GetLeftNode()-hand side expression");
			return false;
		}

		if (node->GetRightNode() != nullptr && node->GetRightNode()->GetTypeDescriptor() == nullptr)
		{
			CompileError("illegal GetRightNode()-hand side expression");
			return false;
		}

		*t1 = (node->GetLeftNode()->GetTypeDescriptor())->GetId();
		*t2 = node->GetRightNode() ? (node->GetRightNode()->GetTypeDescriptor())->GetId() : TypeDescriptor::Id::Int;

		return true;
	}

	/*!
	自動型変換ノードを挿入します
	@param[in]	node	ノードオブジェクト
	*/
	void LocalSemanticAnalyzer::AutoCast(const std::shared_ptr<SyntaxNode>& node)
	{
		if (node == nullptr)
			return;

		ResolveTypeFromChildNode(node);

		if (node->IsNot(SyntaxNode::Id::Assign) && node->GetRightNode() && node->GetRightNode()->GetTypeDescriptor() && node->GetRightNode()->GetTypeDescriptor()->GetId() == TypeDescriptor::Id::Float)
		{
			node->Set(node->GetRightNode()->GetTypeDescriptor());
		}

		if (node->GetLeftNode())
		{
			if (node->GetTypeDescriptor()->GetId() != node->GetLeftNode()->GetTypeDescriptor()->GetId())
				node->SetLeftNode(node->GetLeftNode()->Cast(node->GetTypeDescriptor(), GetTypeDescriptorFactory()));
		}
		if (node->GetRightNode())
		{
			if (node->GetTypeDescriptor()->GetId() != node->GetRightNode()->GetTypeDescriptor()->GetId())
				node->SetRightNode(node->GetRightNode()->Cast(node->GetTypeDescriptor(), GetTypeDescriptorFactory()));
		}
	}

	void LocalSemanticAnalyzer::PostResolverResolve(std::shared_ptr<SyntaxNode> node)
	{
		if (node == nullptr)
			return;
DO_RECURSIVE:
		SetCurrentFileInformation(node);

		switch (node->GetId())
		{
			///////////////////////////////////////////////////////////////////////
			// 定数定義に関するノード
		case SyntaxNode::Id::DefineConstant:
			// TODO:将来的には対応して下さい
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::UndefineConstant:
			// TODO:将来的には対応して下さい
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// メモリレイアウトに関するノード									
		case SyntaxNode::Id::Allocate:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Static:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// 構造に関するノード
		case SyntaxNode::Id::Actor:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Extend:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Module:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Phantom:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Struct:
			/*
			TODO:ローカルスコープ内で宣言された時のみ有効にして下さい
			symbol_open_structure();
			PostResolverResolve(node->GetLeftNode());
			symbol_close_structure(node->GetString());
			*/
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// 関数宣言に関するノード									
		case SyntaxNode::Id::Action:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareArgument:
			// node->GetLeftNode()
			// node->GetRightNode()
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareFunction:
			// node->GetLeftNode()
			// node->GetRightNode()
			// node->GetBodyNode()
			break;

		case SyntaxNode::Id::NativeFunction:
			// node->GetLeftNode()
			// node->GetRightNode()
			// node->GetBodyNode()
			break;

			///////////////////////////////////////////////////////////////////////
			// 変数宣言に関するノード									
		case SyntaxNode::Id::Declarator:
			if (node->GetSymbol() == nullptr)
				node->Set(GetSymbolFactory()->CreateVariable(
					node->GetString(),
					nullptr,
					GetSymbolFactory()->IsOpenBlock(),
					GetSymbolFactory()->IsFunctionOpened(),
					false
				));
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::DeclareVariable:
			ResolveVariableDescription(node, Symbol::MemoryTypeId::Normal, false);
			// node->GetLeftNode()
			// node->GetRightNode()
			PostResolverResolve(node->GetBodyNode());
			break;

		case SyntaxNode::Id::TypeDescription:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			ResolveTypeDescriptionScoped(node);
			break;

		case SyntaxNode::Id::VariableSize:
			// node->GetLeftNode()
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// ブロックを伴う制御に関するノード
		case SyntaxNode::Id::Block:
		{
			const int32_t inDepth = GetSymbolFactory()->OpenBlock(false);
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			const int32_t outDepth = GetSymbolFactory()->CloseBlock();
			MANA_VERIFY_MESSAGE(
				inDepth == outDepth,
				Concat({ "ブロックの深さが一致しません in:", std::to_string(inDepth), " out:", std::to_string(outDepth) })
			);
		}
		MANA_ASSERT(node->GetBodyNode() == nullptr);
		break;

		case SyntaxNode::Id::Break:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Case:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Continue:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Default:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			PostResolverResolve(node->GetBodyNode());
			break;

		case SyntaxNode::Id::Do:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::For:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			PostResolverResolve(node->GetBodyNode());
			break;

		case SyntaxNode::Id::Goto:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Halt:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::If:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			PostResolverResolve(node->GetBodyNode());
			break;

		case SyntaxNode::Id::Label:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Lock:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Loop:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Return:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Rollback:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Switch:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::While:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// 制御に関するノード
		case SyntaxNode::Id::Accept:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Join:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Print:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Reject:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Request:
			PostResolverResolve(node->GetLeftNode());
			ResolveActionReference(node, node->GetString());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::AwaitStart:
		case SyntaxNode::Id::AwaitCompletion:
			PostResolverResolve(node->GetLeftNode());
			ResolveActionReference(node, node->GetString());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			if (const auto& currentBlockTypeDescriptor = GetSymbolFactory()->GetCurrentBlockTypeDescriptor())
			{
				if (currentBlockTypeDescriptor->Compare(node->GetRightNode()->GetTypeDescriptor()))
				{
					CompileError({ "Cannot specify the type of self. Use 'request' instead." });
				}
			}
			break;

		case SyntaxNode::Id::Yield:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Namespace:
			EnterNamespace(node->GetString());
			PostResolverResolve(node->GetLeftNode());
			ExitNamespace();
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Using:
			ResolveUsingDeclaration(node);
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

			///////////////////////////////////////////////////////////////////////
			// 二項演算子に関するノード
		case SyntaxNode::Id::Add:
		case SyntaxNode::Id::Sub:
		case SyntaxNode::Id::Mul:
		case SyntaxNode::Id::Div:
		case SyntaxNode::Id::Rem:
		case SyntaxNode::Id::Pow:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Float || t2 < TypeDescriptor::Id::Char || t2 > TypeDescriptor::Id::Float)
					{
						CompileError("incompatible GetTypeDescriptor() operation in expression");
					}
					else
					{
						AutoCast(node);
						TypeDescriptor::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					}
				}
			}
			break;

		case SyntaxNode::Id::And:
		case SyntaxNode::Id::Or:
		case SyntaxNode::Id::Xor:
		case SyntaxNode::Id::LeftShift:
		case SyntaxNode::Id::RightShift:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int || t2 < TypeDescriptor::Id::Char || t2 > TypeDescriptor::Id::Int)
						CompileError("incompatible GetTypeDescriptor() operation in expression");
				}
			}
			break;

		case SyntaxNode::Id::LogicalAnd:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int || t2 < TypeDescriptor::Id::Char || t2 > TypeDescriptor::Id::Int)
						CompileError("incompatible GetTypeDescriptor() operation in expression");
					// TODO
					//node->Set( = IntermediateLanguage::LogicalAnd;
				}
			}
			break;

		case SyntaxNode::Id::LogicalOr:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int || t2 < TypeDescriptor::Id::Char || t2 > TypeDescriptor::Id::Int)
						CompileError("incompatible GetTypeDescriptor() operation in expression");
					node->Set(IntermediateLanguage::LogicalOr);
				}
			}
			break;

		case SyntaxNode::Id::Equal:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					AutoCast(node);
					TypeDescriptor::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Bool:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::Actor:
						node->Set(IntermediateLanguage::CompareEqualInteger);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(IntermediateLanguage::CompareEqualFloat);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(IntermediateLanguage::CompareEqualData);
						break;
					default:
						MANA_BUG("illegal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

		case SyntaxNode::Id::GreaterEqual:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 == TypeDescriptor::Id::Actor || t2 == TypeDescriptor::Id::Actor)
					{
						CompileError("actor comparisons are limited to == and !=");
						break;
					}
					AutoCast(node);
					TypeDescriptor::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Bool:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::Actor:
						node->Set(IntermediateLanguage::CompareGreaterEqualInteger);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(IntermediateLanguage::CompareGreaterEqualFloat);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(IntermediateLanguage::CompareGreaterEqualData);
						break;
					default:
						MANA_BUG("illegal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

		case SyntaxNode::Id::Greater:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 == TypeDescriptor::Id::Actor || t2 == TypeDescriptor::Id::Actor)
					{
						CompileError("actor comparisons are limited to == and !=");
						break;
					}
					AutoCast(node);
					TypeDescriptor::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Bool:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::Actor:
						node->Set(IntermediateLanguage::CompareGreaterInteger);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(IntermediateLanguage::CompareGreaterFloat);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(IntermediateLanguage::CompareGreaterData);
						break;
					default:
						MANA_BUG("illegal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

		case SyntaxNode::Id::LessEqual:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 == TypeDescriptor::Id::Actor || t2 == TypeDescriptor::Id::Actor)
					{
						CompileError("actor comparisons are limited to == and !=");
						break;
					}
					AutoCast(node);
					TypeDescriptor::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Bool:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::Actor:
						node->Set(IntermediateLanguage::CompareLessEqualInteger);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(IntermediateLanguage::CompareLessEqualFloat);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(IntermediateLanguage::CompareLessEqualData);
						break;
					default:
						MANA_BUG("illegal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

		case SyntaxNode::Id::Less:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 == TypeDescriptor::Id::Actor || t2 == TypeDescriptor::Id::Actor)
					{
						CompileError("actor comparisons are limited to == and !=");
						break;
					}
					AutoCast(node);
					TypeDescriptor::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Bool:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::Actor:
						node->Set(IntermediateLanguage::CompareLessInteger);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(IntermediateLanguage::CompareLessFloat);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(IntermediateLanguage::CompareLessData);
						break;
					default:
						MANA_BUG("illegal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

		case SyntaxNode::Id::NotEqual:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					AutoCast(node);
					TypeDescriptor::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
					switch (t1)
					{
					case TypeDescriptor::Id::Char:
					case TypeDescriptor::Id::Short:
					case TypeDescriptor::Id::Bool:
					case TypeDescriptor::Id::Int:
					case TypeDescriptor::Id::Actor:
						node->Set(IntermediateLanguage::CompareNotEqualInteger);
						break;
					case TypeDescriptor::Id::Float:
						node->Set(IntermediateLanguage::CompareNotEqualFloat);
						break;
					case TypeDescriptor::Id::Struct:
						node->Set(IntermediateLanguage::CompareNotEqualData);
						break;
					default:
						MANA_BUG("illegal data GetTypeDescriptor()");
					}
					node->Set(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char));
				}
			}
			break;

			///////////////////////////////////////////////////////////////////////
			// 単項演算子に関するノード
		case SyntaxNode::Id::LogicalNot:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int)
						CompileError("incompatible GetTypeDescriptor() operation in expression");
					node->Set(IntermediateLanguage::LogicalNot);
				}
			}
			break;

		case SyntaxNode::Id::Not:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int)
						CompileError("incompatible GetTypeDescriptor() operation in expression");
					node->Set(IntermediateLanguage::Not);
				}
			}
			break;

		case SyntaxNode::Id::Neg:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Float)
						CompileError("incompatible GetTypeDescriptor() operation in expression");
				}
			}
			break;

			///////////////////////////////////////////////////////////////////////
			// 演算に関するノード
		case SyntaxNode::Id::Array:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t2 == TypeDescriptor::Id::Void || t2 > TypeDescriptor::Id::Float)
						CompileError("non-integer expression used as subscript");
					if (t1 != TypeDescriptor::Id::Array)
						CompileError("subscript specified to non-array");
					else
					{
						if (node->GetRightNode()->Is(SyntaxNode::Id::Const))
						{
							MANA_ASSERT(
								node->GetRightNode()->GetTypeDescriptor() == GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char) ||
								node->GetRightNode()->GetTypeDescriptor() == GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Short) ||
								node->GetRightNode()->GetTypeDescriptor() == GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int) ||
								node->GetRightNode()->GetTypeDescriptor() == GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Bool));

							if (ToAddress(node->GetRightNode()->GetInt()) >= (node->GetLeftNode()->GetTypeDescriptor())->GetArraySize())
								CompileError("subscript range over");
						}

						node->Set(node->GetLeftNode()->GetTypeDescriptor()->GetComponent());
					}
				}
			}
			break;

		case SyntaxNode::Id::Assign:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);

			if (node->GetLeftNode()->Is(SyntaxNode::Id::Const))
			{
				CompileError({ "already initialized constant '", node->GetLeftNode()->GetSymbol()->GetName(), "'" });
				return;
			}
			AutoCast(node);
			TypeDescriptor::Compatible(node->GetLeftNode()->GetTypeDescriptor(), node->GetRightNode()->GetTypeDescriptor());
			break;

		case SyntaxNode::Id::Call:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				SearchSymbolFromName(node);

				if (node->GetSymbol()->GetClassTypeId() != Symbol::ClassTypeId::Function &&
					node->GetSymbol()->GetClassTypeId() != Symbol::ClassTypeId::NativeFunction &&
					node->GetSymbol()->GetClassTypeId() != Symbol::ClassTypeId::MemberFunction)
				{
					CompileError("trying to call non-function");
				}
			}
			break;

		case SyntaxNode::Id::CallArgument:
			PostResolverResolve(node->GetLeftNode());
			if (node->GetRightNode())
			{
				PostResolverResolve(node->GetRightNode());
				//node->Set( = node->GetRightNode()->Is(SyntaxNode::Id::CallArgument) ? node->GetRightNode()->Set( + 1 : 2;
			}
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Const:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::ExpressionIf:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			PostResolverResolve(node->GetBodyNode());
			break;

		case SyntaxNode::Id::IntegerToFloat:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 < TypeDescriptor::Id::Char || t1 > TypeDescriptor::Id::Int)
						CompileError("incompatible TypeDescriptor operation in expression");
				}
			}
			break;

		case SyntaxNode::Id::FloatToInteger:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			{
				TypeDescriptor::Id t1, t2;
				if (GetNodeType(&t1, &t2, node))
				{
					if (t1 != TypeDescriptor::Id::Float)
						CompileError("incompatible TypeDescriptor operation in expression");
				}
			}
			break;

		case SyntaxNode::Id::Identifier:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);

			if (SearchSymbolFromName(node))
			{
				if (node->GetSymbol())
					node->Set(node->GetSymbol()->GetTypeDescriptor());

				switch (node->GetSymbol()->GetClassTypeId())
				{
				case Symbol::ClassTypeId::ConstantInteger:
					if (node->GetSymbol()->GetTypeDescriptor() == nullptr)
						node->GetSymbol()->SetTypeDescription(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int));
					break;

				case Symbol::ClassTypeId::ConstantFloat:
					node->GetSymbol()->SetTypeDescription(GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float));
					break;

				case Symbol::ClassTypeId::ConstantString:
					node->GetSymbol()->SetTypeDescription(GetTypeDescriptorFactory()->GetString());
					break;

				case Symbol::ClassTypeId::StaticVariable:
					GetSymbolFactory()->IsValid(node->GetSymbol());
					break;

				case Symbol::ClassTypeId::GlobalVariable:
					GetSymbolFactory()->IsValid(node->GetSymbol());
					break;

				case Symbol::ClassTypeId::ActorVariable:
					GetSymbolFactory()->IsValid(node->GetSymbol());
					break;

				case Symbol::ClassTypeId::LocalVariable:
					GetSymbolFactory()->IsValid(node->GetSymbol());
					break;

				case Symbol::ClassTypeId::Type:
					break;

				case Symbol::ClassTypeId::NewSymbol:
				default:
					CompileError("illegal data GetTypeDescriptor()");
					break;
				}
			}
			break;

		case SyntaxNode::Id::MemberFunction:
			PostResolverResolve(node->GetLeftNode());
			PostResolverResolve(node->GetRightNode());
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			abort();
			break;

		case SyntaxNode::Id::MemberVariable:
		{
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);

			std::shared_ptr<TypeDescriptor> type = node->GetLeftNode()->GetTypeDescriptor();
			if (type)
			{
				while (type->Is(TypeDescriptor::Id::Array))
				{
					type = type->GetComponent();
				}
				if (type->Is(TypeDescriptor::Id::Struct))
				{
					while (type)
					{
						for (std::shared_ptr<Symbol> symbol = type->GetSymbolEntry(); symbol; symbol = symbol->GetNext())
						{
							if (symbol->GetName() == node->GetString() && symbol->GetClassTypeId() == Symbol::ClassTypeId::ActorVariable)
							{
								// variable.member
								node->Set(symbol->GetTypeDescriptor());
								goto ESCAPE;
							}
						}
						type = std::reinterpret_pointer_cast<TypeDescriptor>(type->GetParent());
					}
					CompileError({ "reference to undefined field '", node->GetString(), "'" });
				}
				else
				{
					CompileError("reference to non-struct type");
				}
			}
		}
	ESCAPE:
		break;

		case SyntaxNode::Id::Sender:
		case SyntaxNode::Id::Self:
		case SyntaxNode::Id::Priority:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::Sizeof:
			PostResolverResolve(node->GetLeftNode());
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		case SyntaxNode::Id::String:
			MANA_ASSERT(node->GetLeftNode() == nullptr);
			MANA_ASSERT(node->GetRightNode() == nullptr);
			MANA_ASSERT(node->GetBodyNode() == nullptr);
			break;

		default:
			MANA_BUG("illegal node detect");
		}

		// 子ノードから型を継承する
		ResolveTypeFromChildNode(node);

		if (node->GetNextNode())
		{
			// PostResolverResolve(node->GetNextNode()) が末尾再帰なのでgotoにて処理する
			node = node->GetNextNode();
			goto DO_RECURSIVE;
		}
	}
}
