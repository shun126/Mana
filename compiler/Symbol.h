/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "TypeDescriptor.h"

namespace mana
{
	class SymbolFactory;

	/*
	シンボル
	*/
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
			static constexpr std::string_view Names[] = {
				std::string_view("new symbol"),
				std::string_view("typedef"),
				std::string_view("function"),
				std::string_view("native function"),
				std::string_view("member function"),
				std::string_view("static variable"),
				std::string_view("global variable"),
				std::string_view("instance variable"),
				std::string_view("local variable"),
				std::string_view("constant integer"),
				std::string_view("constant real"),
				std::string_view("constant string"),
				std::string_view("label"),
			};
			return Names[static_cast<size_t>(id)];
		}

		enum class MemoryTypeId : uint8_t
		{
			Invalid,
			Normal,
			Parameter
		};

	public:
		~Symbol() = default;

		[[nodiscard]] bool IsValid() const;
		[[nodiscard]] bool IsValidVariable() const;
		
		[[nodiscard]] std::shared_ptr<Symbol> GetParameterList();
		[[nodiscard]] std::shared_ptr<Symbol> GetParameterList() const;
		
		[[nodiscard]] std::shared_ptr<Symbol> GetNext();
		[[nodiscard]] std::shared_ptr<Symbol> GetNext() const;
		
		[[nodiscard]] std::shared_ptr<TypeDescriptor> GetTypeDescriptor();
		[[nodiscard]] std::shared_ptr<TypeDescriptor> GetTypeDescriptor() const;
		
		[[nodiscard]] ClassTypeId GetClassTypeId() const;
		[[nodiscard]] MemoryTypeId GetMemoryTypeId() const;
		
		[[nodiscard]] const std::string& GetName() const;
		[[nodiscard]] int32_t GetAddress() const;
		[[nodiscard]] int32_t GetEtc() const;
		[[nodiscard]] float GetFloat() const;
		[[nodiscard]] std::string_view GetString() const;
		[[nodiscard]] size_t GetBlockLevel() const;
		[[nodiscard]] uint8_t GetNumberOfParameters() const;
		[[nodiscard]] bool IsOverride() const;
		[[nodiscard]] bool IsUsed() const;

		void SetTypeDescription(const std::shared_ptr<TypeDescriptor>& typeDescription);
		
		void SetNumberOfParameters(const size_t numberOfParameters);

		void CheckUndefineRecursive() const;
		
		void OnDump(std::ofstream& output, const int32_t level = 0) const;

	//private:
		Symbol(const std::string_view name, const ClassTypeId class_type, const size_t blockLevel);

		void SetParameterList(const std::shared_ptr<Symbol>& symbol);
		void SetNext(const std::shared_ptr<Symbol>& symbol);
		void SetClassTypeId(const ClassTypeId classTypeId);
		void SetMemoryTypeId(const MemoryTypeId memoryTypeId);
		void SetName(const std::string& name);
		void SetName(const std::string_view& name);
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
