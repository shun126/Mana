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
	class SymbolTable;
	class SymbolFactory;

	class SymbolEntry
	{
		friend class SymbolTable;
		friend class SymbolFactory;

	public:
		enum class ClassTypeId : uint8_t
		{
			NEW_SYMBOL,			//!< 未割り当て関数
			TYPEDEF,			//!< 型定義
			FUNCTION,			//!< 関数
			NATIVE_FUNCTION,	//!< 外部関数
			MEMBER_FUNCTION,	//!< メンバー関数(アクション)
			VARIABLE_STATIC,	//!< スタティックグローバル変数
			VARIABLE_GLOBAL,	//!< グローバル変数
			VARIABLE_ACTOR,		//!< インスタンス変数
			VARIABLE_LOCAL,		//!< ローカル変数
			CONSTANT_INT,		//!< 整数
			CONSTANT_FLOAT,		//!< 実数
			CONSTANT_STRING,	//!< 文字列
			ALIAS,				//!< データ参照
			LABEL,				//!< ラベル
		};

		static const char* GetClassTypeName(const ClassTypeId id)
		{
			static const char* names[] = {
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
				"alias",
				"label",
			};
			return names[static_cast<size_t>(id)];
		}

		enum class MemoryTypeId : uint8_t
		{
			INVALID,
			NORMAL,
			PARAMETER
		};

	public:
		virtual ~SymbolEntry() = default;

		bool IsValidVariable() const;
		
		std::shared_ptr<SymbolEntry> GetParameterList() { return mParameterList; }
		const std::shared_ptr<SymbolEntry> GetParameterList() const { return mParameterList; }
		
		std::shared_ptr<SymbolEntry> GetNext() { return mNext; }
		const std::shared_ptr<SymbolEntry> GetNext() const { return mNext; }
		
		std::shared_ptr<TypeDescriptor> GetTypeDescriptor() { return mTypeDescription; }
		const std::shared_ptr<TypeDescriptor> GetTypeDescriptor() const { return mTypeDescription; }
		
		ClassTypeId GetClassTypeId() const { return mClassTypeId; }
		MemoryTypeId GetMemoryTypeId() const { return mMemoryTypeId; }
		
		const char* GetName() const { return mName; }
		int32_t GetAddress() const { return mAddress; }
		int32_t GetEtc() const { return mEtc; }
		float GetFloat() const { return mFloat; }
		const char* GetString() const { return mString; }
		int32_t GetBlockLevel() const { return mBlockLevel; }
		int32_t GetNumberOfParameters() const { return mNumberOfParameters; }
		bool IsOverride() const { return mOverride; }
		bool IsUsed() const { return mUsed; }

		void SetTypeDescription(const std::shared_ptr<TypeDescriptor>& typeDescription) { mTypeDescription = typeDescription; }
		
		void SetNumberOfParameters(const int32_t numberOfParameters) { mNumberOfParameters = numberOfParameters; }

		void symbol_check_undefine_recursive() const;
		
		void OnDump(std::ofstream& output, const int32_t level = 0) const;

	//private:
		SymbolEntry(const char* name, const ClassTypeId class_type, const int32_t level);
		void SetParameterList(const std::shared_ptr<SymbolEntry>& symbol) { mParameterList = symbol; }
		void SetNext(const std::shared_ptr<SymbolEntry>& symbol) { mNext = symbol; }
		void SetClassTypeId(const ClassTypeId classTypeId) { mClassTypeId = classTypeId; }
		void SetMemoryTypeId(const MemoryTypeId MemoryTypeId) { mMemoryTypeId = MemoryTypeId; }
		void SetName(const char* name) { mName = name; }
		void SetAddress(const int32_t address) { mAddress = address; }
		void SetEtc(const int32_t etc) { mEtc = etc; }
		void SetFloat(const float value) { mFloat = value; }
		void SetString(const char* value) { mString = value; }
		void SetBlockLevel(const int32_t blockLevel) { mBlockLevel = blockLevel; }
		void SetOverride(const bool override_) { mOverride = override_; }
		void SetUsed(const bool used) { mUsed = used; }

	public:
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		char mMagic[8];
#endif
		std::shared_ptr<SymbolEntry> mParameterList;
		std::shared_ptr<SymbolEntry> mNext;
		std::shared_ptr<TypeDescriptor> mTypeDescription;
		ClassTypeId mClassTypeId = ClassTypeId::NEW_SYMBOL;
		MemoryTypeId mMemoryTypeId = MemoryTypeId::INVALID;
		const char* mName = nullptr;
		int32_t mAddress = 0;
		int32_t mEtc = 0;							// TODO:使いまわしを止めてください
		float mFloat = 0;
		const char* mString = nullptr;
		int32_t mBlockLevel = 0;
		int32_t mNumberOfParameters = 0;
		bool mOverride = false;
		bool mUsed = true;
	};
}
