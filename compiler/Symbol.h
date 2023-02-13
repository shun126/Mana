/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "TypeDescriptor.h"
#include <fstream>

namespace mana
{
	class SymbolFactory;

	class Symbol final
	{
		friend class SymbolFactory;

	public:
		enum class ClassTypeId : uint8_t
		{
			NewSymbol,
			Type,
			Function,
			NativeFunction,
			MemberFunction,
			StaticVariable,
			GlobalVariable,
			ActorVariable,
			LocalVariable,
			ConstantInteger,
			ConstantFloat,
			ConstantString,
			Label,
		};

		static const std::string_view& GetClassTypeName(const ClassTypeId id)
		{
			static const std::string_view names[] = {
				"new symbol",
				"typedef",
				"function",
				"native function",
				"member function",
				"static variable",
				"global variable",
				"instance variable",
				"local variable",
				"constant integer",
				"constant real",
				"constant string",
				"label",
			};
			return names[static_cast<size_t>(id)];
		}

		enum class MemoryTypeId : uint8_t
		{
			Invalid,
			Normal,
			Parameter
		};

	public:
		~Symbol() = default;

		bool IsValid() const;
		bool IsValidVariable() const;
		
		std::shared_ptr<Symbol> GetParameterList();
		const std::shared_ptr<Symbol> GetParameterList() const;
		
		std::shared_ptr<Symbol> GetNext();
		const std::shared_ptr<Symbol> GetNext() const;
		
		std::shared_ptr<TypeDescriptor> GetTypeDescriptor();
		const std::shared_ptr<TypeDescriptor> GetTypeDescriptor() const;
		
		ClassTypeId GetClassTypeId() const;
		MemoryTypeId GetMemoryTypeId() const;
		
		const std::string_view GetName() const;
		int32_t GetAddress() const;
		int32_t GetEtc() const;
		float GetFloat() const;
		const std::string_view GetString() const;
		size_t GetBlockLevel() const;
		uint8_t GetNumberOfParameters() const;
		bool IsOverride() const;
		bool IsUsed() const;

		void SetTypeDescription(const std::shared_ptr<TypeDescriptor>& typeDescription);
		
		void SetNumberOfParameters(const size_t numberOfParameters);

		void symbol_check_undefine_recursive() const;
		
		void OnDump(std::ofstream& output, const int32_t level = 0) const;

	//private:
		Symbol(const std::string_view name, const ClassTypeId class_type, const size_t blockLevel);

		void SetParameterList(const std::shared_ptr<Symbol>& symbol);
		void SetNext(const std::shared_ptr<Symbol>& symbol);
		void SetClassTypeId(const ClassTypeId classTypeId);
		void SetMemoryTypeId(const MemoryTypeId MemoryTypeId);
		void SetName(const std::string_view name);
		void SetAddress(const int32_t address);
		void SetEtc(const int32_t etc);
		void SetFloat(const float value);
		void SetString(const std::string_view value);
		void SetBlockLevel(const size_t blockLevel);
		void SetOverride(const bool override_);
		void SetUsed(const bool used);

	public:
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		char mMagic[8];
#endif
		std::shared_ptr<Symbol> mParameterList;
		std::shared_ptr<Symbol> mNext;
		std::shared_ptr<TypeDescriptor> mTypeDescription;
		ClassTypeId mClassTypeId = ClassTypeId::NewSymbol;
		MemoryTypeId mMemoryTypeId = MemoryTypeId::Invalid;
		std::string mName;
		int32_t mAddress = 0;
		int32_t mEtc = 0;					// TODO:使いまわしを止めてください
		float mFloat = 0;
		std::string mString;
		size_t mBlockLevel = 0;
		uint8_t mNumberOfParameters = 0;
		bool mOverride = false;
		bool mUsed = true;
	};
}
