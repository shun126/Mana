/*!
mana (library)

\author	Shun Moriya
\date	2003-
*/

#pragma once
#include "Datalink.h"
#include "Actor.h"
#include "Buffer.h"
#include "Plugin.h"
#include <bitset>
#include <functional>
#include <memory>
#include <unordered_map>

namespace mana
{
	class Actor;

    class VM : private Noncopyable, public std::enable_shared_from_this<VM>
	{
		friend class Actor;

		static const uint32_t nil = static_cast<uint32_t>(~0);

	public:
		//! 外部関数宣言用型
		using ExternalFuntionType = void(*)(const std::shared_ptr<Actor>&);

	public:
		VM();
		virtual ~VM();
		
		void LoadPlugin(const char* filename);
		void LoadPlugins(const char* directoryname);
		void RegistFunction(const char* name, ExternalFuntionType function);

		//void Serialize(mana_stream* stream);
		//void Deserialize(mana_stream* stream);
		
        void LoadProgram(const std::shared_ptr<const void>& program);
		void UnloadProgram();
				
		void Restart();
		bool Run();
		bool IsRunning() const;

		void Execute(std::function<void()> function)
		{
			while (Run())
			{
				function();
			}
		}

		void RequestAll(const uint32_t level, const char* actionName, const std::shared_ptr<Actor>& sender);
		bool Request(const uint32_t level, const char* actorName, const char* actionName, const std::shared_ptr<Actor>& sender);
		void yield();
		const std::shared_ptr<Actor>& GetActor(const char* name);
		const char* GetActorName(const std::shared_ptr<Actor>& actor) const;
		std::shared_ptr<Actor> CloneActor(const std::shared_ptr<Actor>& actor, const char* newName);
		std::shared_ptr<Actor> CreateActor(const char* name, const char* newName);
		std::shared_ptr<Actor> CreateActorFromPhantom(const char* name, const char* newName);
		bool IsInInitAction() const;
		bool IsFinishInitAction() const;
		
		void SetSystemRequest(const bool enable);
		bool IsSystemRequestEnabled() const;
		
		uint32_t GetFrameCounter() const;
		float_t GetDeltaTime() const;
		bool IsFrameChanged() const;
		
		void GetResource(const size_t resouce_id, const void** buffer, size_t* size) const;

	private:
		int8_t GetInt8FromMemory(const uint32_t address) const;
		uint8_t GetUint8FromMemory(const uint32_t address) const;
		int16_t GetInt16FromMemory(const uint32_t address) const;
		uint16_t GetUint16FromMemory(const uint32_t address) const;
		int32_t GetInt32FromMemory(const uint32_t address) const;
		uint32_t GetUint32FromMemory(const uint32_t address) const;
		float GetFloatFromMemory(const uint32_t address) const;
		const char* GetStringFromMemory(const uint32_t address) const;
		//char* GetStringFromData(const uint32_t address);
		int32_t GetOpecode(const uint32_t address) const;

	private:
		ExternalFuntionType FindFunction(const char* functionName) const
		{
			auto i = mFunctionHash.find(functionName);
			if (i == mFunctionHash.end())
			{
				MANA_ERROR("An external function called %s was not found.\n", functionName);
			}
			return i->second;
		}

	private:
		Plugin mPlugin;
		std::shared_ptr<const void> mProgram;
		//std::vector<,*> m,Array;					//!< , オブジェクトへの配列
		std::unordered_map<const char*, ExternalFuntionType, HashValueGenerate, HashValueCompare> mFunctionHash;
		std::unordered_map<const char*, std::shared_ptr<Actor>, HashValueGenerate, HashValueCompare> mActorHash;		//!< , オブジェクトへの連想配列
		std::unordered_map<const char*, const ActorInfoHeader*, HashValueGenerate, HashValueCompare> mPhantomHash;				//!< phantomを表すmana_actor オブジェクトへの連想配列
		Buffer mGlobalVariables;
		Buffer mStaticVariables;
		Datalink mDatalink;

		const FileHeader* mFileHeader = nullptr;
		const uint8_t* mInstructionPool = nullptr;
		const char* mConstantPool = nullptr;
		
		uint32_t mFrameCounter = 0;
		enum Flag : uint8_t
		{
			DebugMode,					//!< デバックモードフラグ
			FrameChanged,				//!< フレーム変更フラグ
			InitializeActionRunning,	//!< initアクション実行中フラグ
			InitializeActionFinished,	//!< initアクション終了フラグ
			Initialized,				//!< 初期化済みフラグ
			Requested,					//!< リクエスト発生済みフラグ
			EnableSystemRequest			//!< システムリクエスト許可フラグ
		};
		std::bitset<8> mFlag;
	};
}

namespace mana
{
	inline VM::VM()
		: mPlugin(shared_from_this())
	{
	}

	inline VM::~VM()
	{
		UnloadProgram();
    }

#if 0
	inline void Serialize(mana* self, mana_stream* stream)
	{
		mana_hash_iterator iterator;

		assert(self && stream);

		{
			mana_hash_iterator_rewind(&iterator);

			while(mana_hash_iterator_hash_next(&self->mActorHash, &iterator))
			{
				mana_actor_serialize(mana_hash_iterator_get_value(&iterator), stream);
			}
	/*
	m,Array
	*/
		}
	#if 0
		mana_stream_push_string(stream, GetActorName(self, self->map_actor));
		mana_stream_push_string(stream, GetActorName(self, self->player_actor));
	#endif
		mana_stream_push_integer(stream, self->mFrameCounter);

		mana_stream_push_unsigned_char(stream, self->mFlag);
	}

	inline void Deserialize(mana* self, mana_stream* stream)
	{
		mana_hash_iterator iterator;

		assert(self && stream);

		{
			mana_hash_iterator_rewind(&iterator);

			while(mana_hash_iterator_hash_next(&self->mActorHash, &iterator))
			{
				mana_actor_deserialize(mana_hash_iterator_get_value(&iterator), stream);
			}
		}
		/*
	m,Array
		*/
	#if 0
		{
			size_t size = mana_stream_get_string_length(stream) + 1;
			void* string = alloca(size);

			mana_stream_pop_string(stream, string, size);

			self->map_actor = GetActor(self, string);
		}

		{
			size_t size = mana_stream_get_string_length(stream) + 1;
			void* string = alloca(size);

			mana_stream_pop_string(stream, string, size);

			self->player_actor = GetActor(self, string);
		}
	#endif
		self->mFrameCounter = mana_stream_pop_integer(stream);

		self->mFlag = mana_stream_pop_unsigned_char(stream);
	}
#endif
	inline void VM::LoadPlugin(const char* filename)
	{
		mPlugin.Load(filename);
	}

	inline void VM::LoadPlugins(const char* directoryname)
	{
		mPlugin.Regist(directoryname);
	}

	inline void VM::RegistFunction(const char* name, ExternalFuntionType function)
	{
		mFunctionHash[name] = function;
	}

	inline void VM::LoadProgram(const std::shared_ptr<const void>& program)
	{
		UnloadProgram();
		
		mProgram = program;

        if(reinterpret_cast<intptr_t>(program.get()) % Datalink::StandardAlignmentSize)
		{
			const std::string message = FormatText(
				"The program address is NOT aligned on %lld-byte boundaries.",
				Datalink::StandardAlignmentSize
			);
			throw std::invalid_argument(message);
		}
		mFileHeader = reinterpret_cast<const FileHeader*>(mProgram.get());;
		if(std::memcmp(SIGNATURE, mFileHeader->mHeader, sizeof(mFileHeader->mHeader)) != 0)
		{
			throw std::invalid_argument("abnormal mana program loaded.");
		}
		if(mFileHeader->mMajorVersion != MAJOR_VERSION || mFileHeader->mMinorVersion != mana::MINOR_VERSION)
		{
			throw std::invalid_argument("file version error.");
		}
		{
#if UINTPTR_MAX == UINT64_MAX
			const uint8_t is64bit = 1 << FileHeader::Flag::Is64bit;
#else
			const uint8_t is64bit = 0;
#endif
			if ((mFileHeader->mFlag & (1 << FileHeader::Flag::Is64bit)) != is64bit)
			{
				throw std::invalid_argument("different bit size by compiled.");
			}
		}

		// グローバルおよびスタティック変数領域を確保します
		mGlobalVariables.Allocate(mFileHeader->mSizeOfGlobalMemory);
		mStaticVariables.Allocate(mFileHeader->mSizeOfStaticMemory);

		const ActorInfoHeader* actorInfo = reinterpret_cast<const ActorInfoHeader*>(mFileHeader + 1);

		for(uint32_t i = 0; i < mFileHeader->mNumberOfActors; ++i)
		{
			const ActionInfoHeader* actionInfo = reinterpret_cast<const ActionInfoHeader*>(actorInfo + 1);
			actorInfo = reinterpret_cast<const ActorInfoHeader*>(actionInfo + actorInfo->mNumberOfActions);
		}

		mConstantPool = reinterpret_cast<const char*>(actorInfo);

		mInstructionPool = reinterpret_cast<const uint8_t*>(mConstantPool + mFileHeader->mSizeOfConstantPool);
#if 0
		if(!(mFileHeader->mFlag & FileHeader::FLAG_COMPILED))
		{
			uint8_t* constantPool = (uint8_t*)mConstantPool;
			uint32_t program_counter = 0;
			while(program_counter < mFileHeader->mSizeOfInstructionPool)
			{
				if (mInstructionPool[program_counter] == MANA_IL_CALL)
				{
					const char* name = GetString(self, &self->mInstructionPool[program_counter + 1]);
					mana_external_funtion_type* function = mana_hash_get(&mana_external_function_hash, name);
					if (function == nullptr)
					{
						throw std::runtime_error(FormatText("An external function called %s is not found.\n", name));
					}
				}
				program_counter += GetInstructionSize(mInstructionPool, program_counter);
			}

			mFileHeader->mFlag |= FileHeader::FLAG_COMPILED;
		}
		{
			int32_t counter = mFileHeader->mFlag >> 2;
			counter++;
			mFileHeader->mFlag &= 0x03;
			mFileHeader->mFlag |= (counter << 2);
		}

		if(mFileHeader->mFlag & FileHeader::FLAG_RESOURCE)
		{
			uint8_t* p = mInstructionPool + mFileHeader->mSiizeOfInstructionPool;
			p = (uint8_t*)(((intptr_t)p + (Datalink::StandardAlignmentSize - 1))
				/ Datalink::StandardAlignmentSize * Datalink::StandardAlignmentSize);
			mDatalink.Load((void*)p);
		}
#endif

		actorInfo = reinterpret_cast<const ActorInfoHeader*>(mFileHeader + 1);

		for(uint32_t i = 0; i < mFileHeader->mNumberOfActors; ++i)
		{
			const char* actor_name = mConstantPool + actorInfo->mName;

			const ActionInfoHeader* actionInfo = reinterpret_cast<const ActionInfoHeader*>(actorInfo + 1);
			//assert(actorInfo->number_of_instances == 1);

			if(actorInfo->mFlag & (1 << ActorInfoHeader::Flag::Phantom))
			{
				mPhantomHash[actor_name] = actorInfo;

				actorInfo = reinterpret_cast<const ActorInfoHeader*>(actionInfo + actorInfo->mNumberOfActions);
			}
			else
			{
				std::shared_ptr<Actor> actor = std::shared_ptr<Actor>(new Actor(shared_from_this(), actorInfo->mVariableSize));

				for (uint16_t j = 0; j < actorInfo->mNumberOfActions; ++j)
				{
					actor->SetAction(mConstantPool + actionInfo->mName, actionInfo->mAddress);
					++actionInfo;
				}

				mActorHash[actor_name] = actor;
				//m,Array[i] = actor;

				actorInfo = reinterpret_cast<const ActorInfoHeader*>(actionInfo);
			}
		}

		// フラグを初期化
		mFlag.set(Flag::InitializeActionRunning);
		mFlag.set(Flag::Initialized);
		mFlag.set(Flag::EnableSystemRequest);

		// 全アクターの initとmain アクションを実行
		Restart();
		RequestAll(1, "init", nullptr);
		RequestAll(0, "main", nullptr);
	}

	inline void VM::UnloadProgram()
	{
		/* スクリプトで確保したリソースの開放 */
		/*
		GetResource().Clear();
		*/

		/* イベントボックスの開放 */
		/*
		DestroyIntersections();
		*/

		// 変数の初期化
		mFlag.reset(Flag::InitializeActionRunning);
		mFlag.reset(Flag::InitializeActionFinished);
		mFlag.reset(Flag::Initialized);
		mFlag.reset(Flag::EnableSystemRequest);

		mActorHash.clear();
		mPhantomHash.clear();

		// 変数領域を解放します
		mGlobalVariables.Reset();
		mStaticVariables.Reset();

		// データの開放
		mDatalink.Unload();

		// プログラムの開放
        mProgram.reset();
		mFileHeader = nullptr;
		mConstantPool = nullptr;
		mInstructionPool = nullptr;
	}

	inline void VM::Restart()
	{
		for (auto& actor : mActorHash)
		{
			actor.second->Restart();
		}
		mFlag.reset(Flag::InitializeActionRunning);
		mFlag.reset(Flag::InitializeActionFinished);
		mFrameCounter = 0;
	}

	inline bool VM::Run()
	{
		bool running = false;

		mFlag.set(Flag::FrameChanged);
		mFlag.reset(Flag::Requested);

		//mana_actor_set_delta_time(second);

		for(auto& actor : mActorHash)
		{
			running |= actor.second->Run();
		}

		mFlag.reset(Flag::FrameChanged);
		while(mFlag[Flag::FrameChanged])
		{
			mFlag.reset(Flag::Requested);

			for(auto& actor : mActorHash)
			{
				if(actor.second->mFlag[Flag::Requested])
				{
					running |= actor.second->Run();
				}
			}
		}
		mFlag.set(Flag::FrameChanged);

		if(mFlag[Flag::InitializeActionRunning] && !running)
		{
			mFlag.set(Flag::InitializeActionFinished);
			mFlag.reset(Flag::InitializeActionRunning);
		}
		++mFrameCounter;

		return running;
	}

	inline bool VM::IsRunning() const
	{
		if(mFlag[Flag::Initialized])
		{
			for(auto& actor : mActorHash)
			{
				if(actor.second->IsRunning())
					return true;
			}
		}
		return false;
	}

	inline void VM::RequestAll(const uint32_t level, const char* actionName, const std::shared_ptr<Actor>& sender)
	{
		for (auto& actor : mActorHash)
		{
			actor.second->Request(level, actionName, sender);
		}
	}

	inline bool VM::Request(const uint32_t level, const char* actorName, const char* actionName, const std::shared_ptr<Actor>& sender)
	{
		const std::shared_ptr<Actor>& actor = mActorHash[actorName];
		if (actor == nullptr)
			return false;
		return actor->Request(level, actionName, sender);
	}

	inline void VM::yield()
	{
		for (auto& actor : mActorHash)
		{
			//actor.second->yield();
		}
	}

	inline const std::shared_ptr<Actor>& VM::GetActor(const char* name)
	{
		return mActorHash[name];
	}

	inline const char* VM::GetActorName(const std::shared_ptr<Actor>& actor) const
	{
		if (mFlag[Flag::Initialized])
		{
			for (const auto& i : mActorHash)
			{
				if (actor.get() == i.second.get())
					return i.first;
			}
		}

		return nullptr;
	}

	inline std::shared_ptr<Actor> VM::CloneActor(const std::shared_ptr<Actor>& actor, const char* newName)
	{
		if (actor->mVM.lock() != shared_from_this())
			throw std::runtime_error("It is not possible to duplicate actors of different VMs");

		if (mActorHash.find(newName) != mActorHash.end())
			throw std::invalid_argument("Symbol has already been registered");

		const std::shared_ptr<Actor>& newActor = actor->Clone();
		mActorHash[newName] = newActor;
		return newActor;
	}

	inline std::shared_ptr<Actor> VM::CreateActor(const char* name, const char* newName)
	{
		return CloneActor(GetActor(name), newName);
	}

	inline std::shared_ptr<Actor> VM::CreateActorFromPhantom(const char* name, const char* newName)
	{
		const ActorInfoHeader* actorInfo = mPhantomHash[name];
		if (actorInfo == nullptr)
			throw std::runtime_error("Phantom not found");

		if (!(actorInfo->mFlag && (1 << ActorInfoHeader::Flag::Phantom)))
			throw std::runtime_error("It is not a phantom");

		std::shared_ptr<Actor> newActor = std::shared_ptr<Actor>(new Actor(shared_from_this(), actorInfo->mVariableSize));
		mActorHash[newName] = std::move(newActor);

		{
			const ActionInfoHeader* actionInfo = reinterpret_cast<const ActionInfoHeader*>(actorInfo + 1);
			for (int32_t j = 0; j < actorInfo->mNumberOfActions; ++j, ++actorInfo)
			{
				const char* actionName = mConstantPool + actionInfo->mName;
				newActor->mActions[actionName] = /*mInstructionPool + */actionInfo->mAddress;
			}
		}

		return newActor;
	}

	inline void VM::GetResource(const size_t resouce_id, const void** buffer, size_t* size) const
	{
		MANA_ASSERT(resouce_id >= 0 && resouce_id < mDatalink.GetNumberOfDatas());
		*buffer = mDatalink.GetData(resouce_id);
		*size = mDatalink.GetDataSize(resouce_id);
	}

	inline uint16_t VM::GetUint16FromMemory(const uint32_t address) const
	{
		uint8_t* pointer;
		uint16_t value;

		MANA_ASSERT(address < mFileHeader->mSizeOfInstructionPool);

		pointer = (uint8_t*)&value;

		if (IsBigEndian())
		{
			pointer[0] = mInstructionPool[address + 0];
			pointer[1] = mInstructionPool[address + 1];
		}
		else
		{
			pointer[1] = mInstructionPool[address + 0];
			pointer[0] = mInstructionPool[address + 1];
		}

		return value;
	}

	inline uint32_t VM::GetUint32FromMemory(const uint32_t address) const
	{
		uint8_t* pointer;
		uint32_t value;

		MANA_ASSERT(address < mFileHeader->mSizeOfInstructionPool);

		pointer = (uint8_t*)&value;

		if (IsBigEndian())
		{
			pointer[0] = mInstructionPool[address + 0];
			pointer[1] = mInstructionPool[address + 1];
			pointer[2] = mInstructionPool[address + 2];
			pointer[3] = mInstructionPool[address + 3];
		}
		else
		{
			pointer[3] = mInstructionPool[address + 0];
			pointer[2] = mInstructionPool[address + 1];
			pointer[1] = mInstructionPool[address + 2];
			pointer[0] = mInstructionPool[address + 3];
		}

		return value;
	}

	inline float VM::GetFloatFromMemory(const uint32_t address) const
	{
		uint8_t* pointer;
		float value;

		MANA_ASSERT(address < mFileHeader->mSizeOfInstructionPool);

		pointer = (uint8_t*)&value;

		if (IsBigEndian())
		{
			pointer[0] = mInstructionPool[address + 0];
			pointer[1] = mInstructionPool[address + 1];
			pointer[2] = mInstructionPool[address + 2];
			pointer[3] = mInstructionPool[address + 3];
		}
		else
		{
			pointer[3] = mInstructionPool[address + 0];
			pointer[2] = mInstructionPool[address + 1];
			pointer[1] = mInstructionPool[address + 2];
			pointer[0] = mInstructionPool[address + 3];
		}

		return value;
	}

	inline const char* VM::GetStringFromMemory(const uint32_t address) const
	{
		MANA_ASSERT(address < mFileHeader->mSizeOfInstructionPool);
		const uint32_t offset = GetUint32FromMemory(address);
		MANA_ASSERT(offset < mFileHeader->mSizeOfConstantPool);
		return mConstantPool + offset;
	}

#if 0
	inline char* ,::GetStringFromData(const uint32_t mAddress) const
	{
		MANA_ASSERT(self);
		MANA_ASSERT(mAddress < mVM->mFileHeader->mSizeOfConstantPool);

		return mConstantPool + mAddress;
	}
#endif

	inline int32_t VM::GetOpecode(const uint32_t address) const
	{
		MANA_ASSERT(address != nil);
		int32_t opecode = mInstructionPool[address];
		MANA_ASSERT(address < mFileHeader->mSizeOfInstructionPool);
		MANA_ASSERT(opecode >= 0 && opecode < MANA_IL_MAX_SIZE);
		return opecode;
	}

	inline bool VM::IsInInitAction() const
	{
		return mFlag[Flag::InitializeActionRunning];
	}

	inline bool VM::IsFinishInitAction() const
	{
		return mFlag[Flag::InitializeActionFinished];
	}

	inline void VM::SetSystemRequest(const bool enable)
	{
		if(enable)
			mFlag.set(Flag::EnableSystemRequest);
		else
			mFlag.reset(Flag::EnableSystemRequest);
	}

	inline bool VM::IsSystemRequestEnabled() const
	{
		return mFlag[Flag::EnableSystemRequest];
	}

	inline uint32_t VM::GetFrameCounter() const
	{
		return mFrameCounter;
	}

	inline bool VM::IsFrameChanged() const
	{
		return mFlag[Flag::FrameChanged];
	}

	inline int8_t VM::GetInt8FromMemory(const uint32_t address) const
	{
		MANA_ASSERT(address < mFileHeader->mSizeOfInstructionPool);
		return static_cast<int8_t>(mInstructionPool[address]);
	}

	inline uint8_t VM::GetUint8FromMemory(const uint32_t address) const
	{
		MANA_ASSERT(address < mFileHeader->mSizeOfInstructionPool);
		return mInstructionPool[address];
	}

	inline int16_t VM::GetInt16FromMemory(const uint32_t address) const
	{
		return static_cast<int16_t>(GetUint16FromMemory(address));
	}

	inline int32_t VM::GetInt32FromMemory(const uint32_t address) const
	{
		return static_cast<int32_t>(GetUint32FromMemory(address));
	}
}
