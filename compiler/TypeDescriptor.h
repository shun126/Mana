/*!
mana (compiler)

\author	Shun Moriya
\date	2003-
*/

#pragma once
#include "../runner/common/Platform.h"
#include <array>
#include <fstream>
#include <memory>
#include <string_view>

namespace mana
{
	class Symbol;

	class TypeDescriptor final
	{
		friend class SemanticAnalyzer;
		friend class SymbolFactory;
		friend class TypeDescriptorFactory;

	public:
		//! type identifier
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
			Actor,
			Module,
			Nil,
			Incomplete
		};

		//! type identifier size
		static constexpr size_t TypeIdSize = static_cast<size_t>(Id::Incomplete) + 1;

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
		~TypeDescriptor() = default;

		bool Is(const Id id) const;
		bool IsNot(const Id id) const;
		Id GetId() const;

		bool Compare(const std::shared_ptr<TypeDescriptor>& typeDescriptor) const;
		bool Compatible(const std::shared_ptr<TypeDescriptor>& typeDescriptor) const;

		static bool Compare(const std::shared_ptr<TypeDescriptor>& left, const std::shared_ptr<TypeDescriptor>& right);
		static bool Compatible(const std::shared_ptr<TypeDescriptor>& left, const std::shared_ptr<TypeDescriptor>& right);

		void SetArray(const std::shared_ptr<TypeDescriptor>& arrayElementTypeDescriptor);

		const std::string_view GetName() const;

		std::shared_ptr<Symbol> GetSymbolEntry() const;

		std::shared_ptr<TypeDescriptor> GetComponent() const;

		address_t GetArraySize() const;
		address_t GetMemorySize() const;
		address_t GetAlignmentMemorySize() const;

		address_t GetActionCount() const;
		bool IsPhantom() const;

		const std::shared_ptr<Symbol>& GetParent() const;
		void SetParent(const std::shared_ptr<Symbol>& parent);

		void Dump(std::ofstream& output) const;

	private:
		explicit TypeDescriptor(const Id tcons);
		void SetTypeDescriptor(const std::shared_ptr<TypeDescriptor>& component);
		void SetName(const std::string_view name);
		void SetSymbolEntry(const std::shared_ptr<Symbol>& symbolEntry);
		void SetMemorySize(const address_t memorySize);
		void SetAlignmentMemorySize(const address_t alignmentMemorySize);
		void SetArraySize(const address_t arraySize);


	private:
#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
		char magic[8];
#endif
		std::shared_ptr<Symbol> mSymbolEntry;
		std::shared_ptr<Symbol> mParent;
		std::shared_ptr<TypeDescriptor> mComponent;	//!< Array, link to original type of reference type
		std::string mName;							//!< Type name
		Id mTcons = Id::Incomplete;					//!< Type identifier
		address_t mArraySize = 1;
		address_t mMemorySize = 0;
		address_t mAlignmentMemorySize = 0;

		union share
		{
			struct actor
			{
				bool mPhantom;
			} mActor;
		} mShare;
	};
}
