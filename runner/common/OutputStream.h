/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Platform.h"
#include "String.h"
#include <fstream>
#include <cstring>
#include <memory>
#include <stdexcept>

namespace mana
{
	class OutputStream : Noncopyable
	{
	public:
		OutputStream();
		virtual ~OutputStream() = default;

		void Load(const std::string& filename);
		void Save(const std::string& filename);
	
		const void* GetBuffer() const;
		const void* IndexAt(const size_t index) const;
		size_t GetAllocatedSize() const;
		size_t GetUsedSize() const;

		template<typename T>
		void Push(const T value);
		void PushString(const char* text);
		void PushData(const void* pointer, const size_t size);

		template<typename T>
		T Pop();
		void PopString(char* pointer, const size_t size);
		void PopData(void* pointer, const size_t size);

		template<typename T>
		T Get();
		size_t GetString(char* pointer, const size_t size);
		const char* GetStringPointer() const;
		size_t GetStringLength() const;
	 	void GetData(void* pointer, const size_t size);

		void Rewind();
		void Seek(const ssize_t offset);
		
		std::shared_ptr<const void> MakeShared() const;

	private:
		void ResizeBuffer(const size_t size);

	private:
		std::unique_ptr<uint8_t, decltype(&std::free)> mBuffer;
		size_t mAllocatedSize = 0;
		size_t mUsedSize = 0;
		size_t mPointer = 0;
	};
}

#include "OutputStream.inl"
