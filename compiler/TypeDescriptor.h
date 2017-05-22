/*!
mana (compiler)

¥file	TypeDescriptor.h
¥brief	型情報に関するヘッダーファイル
¥detail	このファイルは型情報に関係するヘッダーファイルです。
¥author	Shun Moriya
¥date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#include <array>
#include <fstream>
#include <memory>

namespace mana
{
	class SymbolEntry;

	/*!
	型識別子クラス
	*/
	class TypeDescriptor
	{
		friend class SymbolFactory;
		friend class SymbolTable;
		friend class TypeDescriptorFactory;

	public:
		//! 型識別子
		enum class Id : uint8_t
		{
			Void,
			Char,
			Short,
			Int,
			Float,
			Reference,
			Array,
			Struct,
			ACTOR,
			MODULE,
			NIL,
			INCOMPLETE
		};

		//! 型識別子の種類
		static constexpr size_t TypeIdSize = static_cast<size_t>(Id::INCOMPLETE) + 1;

		/*!
		型の名称を取得します
		\param[in]	id	型識別子
		*/
		static const char* GetDataTypeName(const Id id)
		{
			static const std::array<const char*, TypeIdSize> names = { {
				"void",
				"char",
				"short",
				"int",
				"float",
				"ref",
				"array",
				"struct",
				"actor",
				"module",
				"nil",
				"incomplete"
			} };
			return names.at(static_cast<size_t>(id));
		}

	public:
		/*!
		コンストラクタ
		\param[in]	tcons	型識別子
		*/
		explicit TypeDescriptor(const Id tcons);

		/*!
		デストラクタ
		*/
		virtual ~TypeDescriptor() = default;

		void SetArray(const std::shared_ptr<TypeDescriptor>& ElmnT);

		bool Compare(const std::shared_ptr<TypeDescriptor>& typeDescriptor) const;
		bool Compatible(const std::shared_ptr<TypeDescriptor>& typeDescriptor) const;

		bool Is(const Id id) const { return mTcons == id; }
		bool IsNot(const Id id) const { return mTcons != id; }
		Id GetId() const { return mTcons; }

		const char* GetName() const { return mName; }
		std::shared_ptr<SymbolEntry> GetSymbolEntry() const { return mSymbolEntry; }
		std::shared_ptr<TypeDescriptor> GetComponent() const { return mComponent; }
		size_t GetArraySize() const { return mArraySize; }
		size_t GetMemorySize() const { return mMemorySize; }
		size_t GetAlignmentMemorySize() const { return mAlignmentMemorySize; }

		void SetTypeDescriptor(const std::shared_ptr<TypeDescriptor>& component) { mComponent = component; }

		size_t GetActionCount() const;
		bool IsPhantom() const;

		void Dump(std::ofstream& output) const;

	private:
		void SetSymbolEntry(const std::shared_ptr<SymbolEntry>& symbolEntry) { mSymbolEntry = symbolEntry; }
		void SetName(const char* name) { mName = name; }
		void SetMemorySize(const size_t memorySize) { mMemorySize = memorySize; }
		void SetAlignmentMemorySize(const size_t alignmentMemorySize) { mAlignmentMemorySize = alignmentMemorySize; }

	private:
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		char magic[8];
#endif
		std::shared_ptr<SymbolEntry> mSymbolEntry;	//!<
		std::shared_ptr<TypeDescriptor> mParent;	//!< 親の型へのリンク(継承がなくなったので未使用)
		std::shared_ptr<TypeDescriptor> mComponent;	//!< 配列、参照型の元の型へのリンク
		Id mTcons = Id::INCOMPLETE;					//!< 型のID
		const char* mName = nullptr;				//!< 型の名称
		size_t mArraySize = 1;						//!< 配列のサイズ
		size_t mMemorySize = 0;						//!< メモリサイズ
		size_t mAlignmentMemorySize = 0;			//!< アライメントサイズ

		union share
		{
			struct actor
			{
				int32_t mPhantom;
			}actor;
		}share;
	};
}
