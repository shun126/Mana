/*!
mana (library)

\author	Shun Moriya
\date	2003-
*/

#pragma once
#include "Plugin.h"

namespace mana
{
	inline VM::VM()
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

			while (mana_hash_iterator_hash_next(&self->mActorHash, &iterator))
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

			while (mana_hash_iterator_hash_next(&self->mActorHash, &iterator))
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
	inline void VM::LoadPlugin(const std::string& filename)
	{
		if (mPlugin == nullptr)
			mPlugin = std::make_shared<Plugin>(shared_from_this());
		mPlugin->Load(filename);
	}

	inline void VM::LoadPlugins(const std::string& directoryName)
	{
		if (mPlugin == nullptr)
			mPlugin = std::make_shared<Plugin>(shared_from_this());
		mPlugin->Register(directoryName);
	}

	inline void VM::RegisterFunction(const std::string& name, const ExternalFunctionType& function)
	{
		mFunctionHash[name] = function;
	}

	inline VM::ExternalFunctionType VM::FindFunction(const std::string& functionName) const
	{
		auto i = mFunctionHash.find(functionName);
		if (i == mFunctionHash.end())
		{
			MANA_ERROR({ "An external function called ", functionName, " was not found.\n" });
		}
		return i->second;
	}

	inline void VM::LoadProgram(const std::shared_ptr<const void>& program)
	{
		UnloadProgram();

		mProgram = program;

		mFileHeader = reinterpret_cast<const FileHeader*>(mProgram.get());
		if (std::memcmp(Signature, mFileHeader->mHeader, sizeof(mFileHeader->mHeader)) != 0)
		{
			throw std::invalid_argument("abnormal mana program loaded.");
		}
		if (mFileHeader->mMajorVersion != MajorVersion || mFileHeader->mMinorVersion != mana::MinorVersion)
		{
			throw std::invalid_argument("file version error.");
		}
		{
#if UINTPTR_MAX == UINT64_MAX
			const uint8_t is64bit = 1 << FileHeader::Flag::Is64Bit;
#else
			const uint8_t is64bit = 0;
#endif
			if ((mFileHeader->mFlag & (1 << FileHeader::Flag::Is64Bit)) != is64bit)
			{
				throw std::invalid_argument("different bit size by compiled.");
			}
		}

		// ・ｽO・ｽ・ｽ・ｽ[・ｽo・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾑス・ｽ^・ｽe・ｽB・ｽb・ｽN・ｽﾏ撰ｿｽ・ｽﾌ茨ｿｽ・ｽ・ｽm・ｽﾛゑｿｽ・ｽﾜゑｿｽ
		mGlobalVariables.Allocate(mFileHeader->mSizeOfGlobalMemory);
		mStaticVariables.Allocate(mFileHeader->mSizeOfStaticMemory);

		const ActorInfoHeader* actorInfo = reinterpret_cast<const ActorInfoHeader*>(mFileHeader + 1);

		for (uint32_t i = 0; i < mFileHeader->mNumberOfActors; ++i)
		{
			const ActionInfoHeader* actionInfo = reinterpret_cast<const ActionInfoHeader*>(actorInfo + 1);
			actorInfo = reinterpret_cast<const ActorInfoHeader*>(actionInfo + actorInfo->mNumberOfActions);
		}

		mConstantPool = reinterpret_cast<const char*>(actorInfo);

		mInstructionPool = reinterpret_cast<const uint8_t*>(mConstantPool + mFileHeader->mSizeOfConstantPool);
#if 0
		if (!(mFileHeader->mFlag & FileHeader::FLAG_COMPILED))
		{
			uint8_t* constantPool = (uint8_t*)mConstantPool;
			uint32_t program_counter = 0;
			while (program_counter < mFileHeader->mSizeOfInstructionPool)
			{
				if (mInstructionPool[program_counter] == Call)
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
#endif

		actorInfo = reinterpret_cast<const ActorInfoHeader*>(mFileHeader + 1);

		for (uint32_t i = 0; i < mFileHeader->mNumberOfActors; ++i)
		{
			const std::string_view actorName = mConstantPool + actorInfo->mName;

			const ActionInfoHeader* actionInfo = reinterpret_cast<const ActionInfoHeader*>(actorInfo + 1);
			//assert(actorInfo->number_of_instances == 1);

			if (actorInfo->mFlag & (1 << ActorInfoHeader::Flag::Phantom))
			{
				mPhantomHash[actorName] = actorInfo;

				actorInfo = reinterpret_cast<const ActorInfoHeader*>(actionInfo + actorInfo->mNumberOfActions);
			}
			else
			{
				std::shared_ptr<Actor> actor = std::shared_ptr<Actor>(new Actor(shared_from_this(), actorInfo->mVariableSize));

#if MANA_BUILD_TARGET < MANA_BUILD_RELEASE
				actor->SetActorName(actorName);
#endif

				for (uint16_t j = 0; j < actorInfo->mNumberOfActions; ++j)
				{
					actor->SetAction(mConstantPool + actionInfo->mName, actionInfo->mAddress);
					++actionInfo;
				}

				mActorHash[actorName] = actor;

				actorInfo = reinterpret_cast<const ActorInfoHeader*>(actionInfo);
			}
		}

		// ・ｽt・ｽ・ｽ・ｽO・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
		mFlag.set(Flag::InitializeActionRunning);
		mFlag.set(Flag::Initialized);
		mFlag.set(Flag::EnableSystemRequest);

		// ・ｽS・ｽA・ｽN・ｽ^・ｽ[・ｽ・ｽ init・ｽ・ｽmain ・ｽA・ｽN・ｽV・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽs
		Restart();
		RequestAll(1, "init", nullptr);
		RequestAll(0, "main", nullptr);
	}

	inline void VM::UnloadProgram()
	{
		/* ・ｽX・ｽN・ｽ・ｽ・ｽv・ｽg・ｽﾅ確・ｽﾛゑｿｽ・ｽ・ｽ・ｽ・ｽ・ｽ\・ｽ[・ｽX・ｽﾌ開・ｽ・ｽ */
		/*
		GetResource().Clear();
		*/

		/* ・ｽC・ｽx・ｽ・ｽ・ｽg・ｽ{・ｽb・ｽN・ｽX・ｽﾌ開・ｽ・ｽ */
		/*
		DestroyIntersections();
		*/

		// ・ｽﾏ撰ｿｽ・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽ・ｽ
		mFlag.reset(Flag::InitializeActionRunning);
		mFlag.reset(Flag::InitializeActionFinished);
		mFlag.reset(Flag::Initialized);
		mFlag.reset(Flag::EnableSystemRequest);

		mActorHash.clear();
		mPhantomHash.clear();

		// ・ｽﾏ撰ｿｽ・ｽﾌ茨ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ
		mGlobalVariables.Reset();
		mStaticVariables.Reset();

		// ・ｽv・ｽ・ｽ・ｽO・ｽ・ｽ・ｽ・ｽ・ｽﾌ開・ｽ・ｽ
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

		for (auto& actor : mActorHash)
		{
			running |= actor.second->Run();
		}

		mFlag.reset(Flag::FrameChanged);
		while (mFlag[Flag::FrameChanged])
		{
			mFlag.reset(Flag::Requested);

			for (auto& actor : mActorHash)
			{
				if (actor.second->mFlag[Flag::Requested])
				{
					running |= actor.second->Run();
				}
			}
		}
		mFlag.set(Flag::FrameChanged);

		if (mFlag[Flag::InitializeActionRunning] && !running)
		{
			mFlag.set(Flag::InitializeActionFinished);
			mFlag.reset(Flag::InitializeActionRunning);
		}
		++mFrameCounter;

		return running;
	}

	inline bool VM::IsRunning() const
	{
		if (mFlag[Flag::Initialized])
		{
			for (auto& actor : mActorHash)
			{
				if (actor.second->IsRunning())
					return true;
			}
		}
		return false;
	}

	inline void VM::Execute(const std::function<void()>& function)
	{
		while (Run())
		{
			function();
		}
	}

	inline void VM::RequestAll(const int32_t level, const char* actionName, const std::shared_ptr<Actor>& sender) const
	{
		for (auto& actor : mActorHash)
		{
			actor.second->Request(level, actionName, sender);
		}
	}

	inline bool VM::Request(const int32_t level, const char* actorName, const char* actionName, const std::shared_ptr<Actor>& sender)
	{
		const std::shared_ptr<Actor>& actor = mActorHash[actorName];
		if (actor == nullptr)
			return false;
		return actor->Request(level, actionName, sender);
	}

	inline void VM::YieldAll()
	{
		for (auto& actor : mActorHash)
		{
			actor.second->yield();
		}
	}

	inline const std::shared_ptr<Actor>& VM::GetActor(const char* name)
	{
		return mActorHash[name];
	}

	inline const std::string_view& VM::GetActorName(const std::shared_ptr<Actor>& actor) const
	{
		if (mFlag[Flag::Initialized])
		{
			for (const auto& i : mActorHash)
			{
				if (actor.get() == i.second.get())
					return i.first;
			}
		}

		static const std::string_view empty;
		return empty;
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

		if ((actorInfo->mFlag & (1 << ActorInfoHeader::Flag::Phantom)) == 0)
			throw std::runtime_error("It is not a phantom");

		std::shared_ptr<Actor> newActor = std::shared_ptr<Actor>(
			new Actor(shared_from_this(), actorInfo->mVariableSize)
		);
		mActorHash[newName] = newActor;

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
	inline char*, ::GetStringFromData(const uint32_t mAddress) const
	{
		MANA_ASSERT(self);
		MANA_ASSERT(mAddress < mVM->mFileHeader->mSizeOfConstantPool);

		return mConstantPool + mAddress;
	}
#endif

	inline int32_t VM::GetOpecode(const uint32_t address) const
	{
		MANA_ASSERT(address != Nil);
		int32_t opecode = mInstructionPool[address];
		MANA_ASSERT(address < mFileHeader->mSizeOfInstructionPool);
		MANA_ASSERT(opecode >= 0 && opecode < IntermediateLanguageSize);
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
		if (enable)
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

	inline Buffer& VM::GetGlobalVariables() noexcept
	{
		return mGlobalVariables;
	}

	inline const Buffer& VM::GetGlobalVariables() const noexcept
	{
		return mGlobalVariables;
	}

	inline Buffer& VM::GetStaticVariables() noexcept
	{
		return mStaticVariables;
	}

	inline const Buffer& VM::GetStaticVariables() const noexcept
	{
		return mStaticVariables;
	}
}
