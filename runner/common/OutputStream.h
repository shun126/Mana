/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Platform.h"
#include <memory>

namespace mana
{
	class OutputStream final : Noncopyable
	{
	public:
		OutputStream();
		OutputStream(const OutputStream& other) = delete;
		OutputStream(OutputStream&& other) noexcept;
		OutputStream& operator=(const OutputStream& other) = delete;
		OutputStream& operator=(OutputStream&& other) noexcept;
		~OutputStream() = default;

		void Load(const std::string& filename);
		void Save(const std::string& filename) const;
	
		[[nodiscard]] const void* GetBuffer() const;
		[[nodiscard]] const void* IndexAt(const size_t index) const;
		[[nodiscard]] size_t GetAllocatedSize() const;
		[[nodiscard]] size_t GetUsedSize() const;

		template<typename T>
		void Push(const T value);
		void PushString(const char* text);
		void PushData(const void* pointer, const size_t size);

		template<typename T>
		[[nodiscard]] T Pop();
		void PopString(char* pointer, const size_t size);
		void PopData(void* pointer, const size_t size);

		template<typename T>
		[[nodiscard]] T Get();
		[[nodiscard]] size_t GetString(char* pointer, const size_t size) const;
		[[nodiscard]] const char* GetStringPointer() const;
		[[nodiscard]] size_t GetStringLength() const;
	 	void GetData(void* pointer, const size_t size) const;

		void Rewind();
		void Seek(const ssize_t offset);
		
		[[nodiscard]] std::shared_ptr<const void> MakeShared() const;

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
