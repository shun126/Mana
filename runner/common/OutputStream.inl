/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "String.h"
#include <fstream>
#include <cstring>

namespace mana
{
	inline OutputStream::OutputStream()
		: mBuffer(nullptr, std::free)
	{
	}

	inline void OutputStream::ResizeBuffer(const size_t size)
	{
		static constexpr size_t PageSize = 1024;

		if (mAllocatedSize < mUsedSize + size)
		{
			const size_t newSize = (mAllocatedSize + size + PageSize - 1) / PageSize * PageSize;
			uint8_t* newBuffer = static_cast<uint8_t*>(realloc(mBuffer.get(), newSize));
			if (newBuffer == nullptr)
				throw std::bad_alloc();
			mBuffer.release();
			mBuffer.reset(newBuffer);
			mAllocatedSize = newSize;
		}
	}

	inline void OutputStream::Load(const std::string& filename)
	{
		std::ifstream infile(filename, std::ios::in | std::ios::ate);

		if (!infile.is_open())
			throw std::runtime_error("file open failed");

		//infile.seekg(std::fstream::end);
		const size_t filesize = static_cast<size_t>(infile.tellg());
		infile.clear();
		infile.seekg(0, std::fstream::beg);

		ResizeBuffer(filesize);

		char* weakBuffer = reinterpret_cast<char*>(mBuffer.get());
		infile.read(weakBuffer, filesize);
		//	throw std::runtime_error("file read error");

		mUsedSize += filesize;
	}

	inline void OutputStream::Save(const std::string& filename) const
	{
		std::ofstream outfile(filename, std::ios::out);
		if (!outfile.is_open())
			throw std::runtime_error("file open failed");

		char* weakBuffer = reinterpret_cast<char*>(mBuffer.get());
		outfile.write(weakBuffer, mUsedSize);
		//	throw std::runtime_error("file write error");
	}

	inline const void* OutputStream::GetBuffer() const
	{
		return static_cast<const void*>(mBuffer.get());
	}

	inline const void* OutputStream::IndexAt(const size_t index) const
	{
		if (index >= GetUsedSize())
			throw std::out_of_range("Buffer overrun");

		const uint8_t* weakBuffer = mBuffer.get();
		return &weakBuffer[index];
	}

	inline size_t OutputStream::GetAllocatedSize() const
	{
		return mAllocatedSize;
	}
	
	inline size_t OutputStream::GetUsedSize() const
	{
		return mUsedSize;
	}

	template<typename T>
	inline void OutputStream::Push(const T value)
	{
		PushData(&value, sizeof(value));
	}

	inline void OutputStream::PushString(const char* text)
	{
		const size_t size = std::strlen(text) + 1;
		ResizeBuffer(size);
		char* weakBuffer = reinterpret_cast<char*>(mBuffer.get());
		strcpy(&weakBuffer[mUsedSize], size, text);
		mUsedSize += size;
	}

	inline void OutputStream::PushData(const void* pointer, const size_t size)
	{
		ResizeBuffer(size);
		uint8_t* weakBuffer = mBuffer.get();
		std::memcpy(&weakBuffer[mUsedSize], pointer, size);
		mUsedSize += size;
	}

	template<typename T>
	inline T OutputStream::Pop()
	{
		T value;
		PopData(&value, sizeof(value));
		return value;
	}

	inline void OutputStream::PopString(char* pointer, const size_t size)
	{
		mPointer += GetString(pointer, size);
	}

	inline void OutputStream::PopData(void* pointer, const size_t size)
	{
		GetData(pointer, size);
		mPointer += size;
	}

	template<typename T>
	inline T OutputStream::Get()
	{
		T value;
		GetData(&value, sizeof(value));
		return value;
	}

	inline size_t OutputStream::GetString(char* pointer, const size_t size) const
	{
		if (mPointer >= mUsedSize)
			throw std::range_error("buffer over");

		char* weakBuffer = reinterpret_cast<char*>(mBuffer.get());

		size_t copy_length = std::strlen(&weakBuffer[mPointer]) + 1;
		if (copy_length > size)
			copy_length = size;

		std::memcpy(pointer, &weakBuffer[mPointer], copy_length);

		return copy_length;
	}

	inline const char* OutputStream::GetStringPointer() const
	{
		if (mPointer >= mUsedSize)
			throw std::range_error("GetStringPointer");
		const char* weakBuffer = reinterpret_cast<const char*>(mBuffer.get());
		return &weakBuffer[mPointer];
	}

	inline size_t OutputStream::GetStringLength() const
	{
		if (mPointer >= mUsedSize)
			throw std::range_error("GetStringLength");
		const char* weakBuffer = reinterpret_cast<const char*>(mBuffer.get());
		return std::strlen(&weakBuffer[mPointer]);
	}

	inline void OutputStream::GetData(void* pointer, const size_t size) const
	{
		if (mPointer >= mUsedSize)
			throw std::range_error("GetData");
		const uint8_t* weakBuffer = mBuffer.get();
		std::memcpy(pointer, &weakBuffer[mPointer], size);
	}

	inline void OutputStream::Rewind()
	{
		mPointer = 0;
	}

	inline void OutputStream::Seek(const ssize_t offset)
	{
		size_t pointer = (size_t)(mPointer + offset);
		if (pointer >= mUsedSize)
			pointer = mUsedSize;
		mPointer = pointer;
	}

	inline std::shared_ptr<const void> OutputStream::MakeShared() const
	{
		uint8_t* newBuffer = static_cast<uint8_t*>(malloc(mUsedSize));
		if (newBuffer == nullptr)
			throw std::bad_alloc();
		std::memcpy(newBuffer, mBuffer.get(), mUsedSize);
		return { newBuffer, std::free };
	}
}
