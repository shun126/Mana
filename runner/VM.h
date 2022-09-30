/*!
mana (library)

\author	Shun Moriya
\date	2003-
*/

#pragma once
#include "Buffer.h"
#include "Datalink.h"
#include <bitset>
#include <functional>
#include <memory>
#include <unordered_map>

namespace mana
{
	class Actor;
	class Plugin;

    class VM : private Noncopyable, public std::enable_shared_from_this<VM>
	{
		friend class Actor;

		static const uint32_t nil = static_cast<uint32_t>(~0);

	public:
		using ExternalFuntionType = std::function<void(const std::shared_ptr<Actor>&)>;

	public:
		VM();
		virtual ~VM();
		
		void LoadPlugin(const std::string& filename);
		void LoadPlugins(const std::string& directoryname);

		void RegistFunction(const std::string& name, ExternalFuntionType function);

#if 0
		void Serialize(mana_stream* stream);
		void Deserialize(mana_stream* stream);
#endif
		
        void LoadProgram(const std::shared_ptr<const void>& program);
		void UnloadProgram();
				
		void Restart();
		bool Run();
		bool IsRunning() const;

		void Execute(std::function<void()> function);

		void RequestAll(const uint32_t level, const char* actionName, const std::shared_ptr<Actor>& sender);
		bool Request(const uint32_t level, const char* actorName, const char* actionName, const std::shared_ptr<Actor>& sender);
		void yield();
		const std::shared_ptr<Actor>& GetActor(const char* name);
		const std::string_view& GetActorName(const std::shared_ptr<Actor>& actor) const;
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
		ExternalFuntionType FindFunction(const std::string& functionName) const;

	private:
		std::shared_ptr<Plugin> mPlugin;
		std::shared_ptr<const void> mProgram;
		const FileHeader* mFileHeader = nullptr;
		const uint8_t* mInstructionPool = nullptr;
		const char* mConstantPool = nullptr;
		//std::vector<,*> m,Array;					//!< , オブジェクトへの配列
		std::unordered_map<std::string, ExternalFuntionType> mFunctionHash;
		std::unordered_map<std::string_view, std::shared_ptr<Actor>> mActorHash;		//!< , オブジェクトへの連想配列
		std::unordered_map<std::string_view, const ActorInfoHeader*> mPhantomHash;		//!< phantomを表すmana_actor オブジェクトへの連想配列
		Buffer mGlobalVariables;
		Buffer mStaticVariables;
		Datalink mDatalink;
		
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

#include "VM.inl"
