/*!
mana (compiler/library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "common/Setup.h"

namespace mana
{
    class Datalink : private Noncopyable
	{        
    public:
        static const size_t StandardAlignmentSize = sizeof(void*);
        
        struct FileHeader
        {
            uint32_t mTotalDataSize;
            uint32_t mNumberOfDatas;
        };
        
        struct InfoHeader
        {
            uint32_t mOffset;
            uint32_t mSize;
        };

    public:
        Datalink() = default;
        virtual ~Datalink() = default;

        void Load(const std::shared_ptr<const void>& buffer);
        void Unload();

		size_t GetNumberOfDatas() const;
        size_t GetDataSize(const size_t index) const;
        const void* GetData(const size_t index) const;

	private:
        std::shared_ptr<const void> mBuffer;
		const InfoHeader* mInfoHeaderPointer;
		const uint8_t* mDataPointer;
	};

	inline void Datalink::Load(const std::shared_ptr<const void>& buffer)
    {
        mBuffer = buffer;
        
        const FileHeader* p = reinterpret_cast<const FileHeader*>(mBuffer.get());
        mInfoHeaderPointer = reinterpret_cast<const InfoHeader*>(&p[1]);
        
        size_t offset = sizeof(FileHeader);
        offset += sizeof(InfoHeader) * p->mNumberOfDatas;
        offset %= StandardAlignmentSize;
        if (offset > 0)
            offset = StandardAlignmentSize - offset;
        
        mDataPointer = (uint8_t*)(&mInfoHeaderPointer[p->mNumberOfDatas]) + offset;
    }

    inline void Datalink::Unload()
    {
        mInfoHeaderPointer = nullptr;
        mDataPointer = nullptr;
        mBuffer.reset();
    }

    inline size_t Datalink::GetNumberOfDatas() const
    {
        const FileHeader* p = reinterpret_cast<const FileHeader*>(mBuffer.get());
        return p->mNumberOfDatas;
    }

    inline size_t Datalink::GetDataSize(const size_t index) const
    {
        MANA_ASSERT_MESSAGE(index < GetNumberOfDatas(), "out of range");
        return mInfoHeaderPointer[index].mSize;
    }

    inline const void* Datalink::GetData(const size_t index) const
    {
        MANA_ASSERT_MESSAGE(index < GetNumberOfDatas(), "out of range");
        return mDataPointer + mInfoHeaderPointer[index].mOffset;
    }
}
