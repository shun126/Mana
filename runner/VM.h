/*!
mana (library)

\author	Shun Moriya
\date	2003-
*/

#pragma once
#include "Buffer.h"
#include <bitset>
#include <functional>
#include <memory>
#include <unordered_map>

namespace mana
{
	class Actor;
	class Plugin;

    class VM final : Noncopyable, public std::enable_shared_from_this<VM>
	{
		friend class Actor;

		static constexpr uint32_t Nil = static_cast<uint32_t>(~0);

	public:
		using ExternalFunctionType = std::function<void(const std::shared_ptr<Actor>&)>;

	public:
		VM();
		VM(const VM&) = delete;
		VM(VM&&) noexcept = delete;
		VM& operator=(const VM&) = delete;
		VM& operator=(VM&&) noexcept = delete;
		~VM();
		
		void LoadPlugin(const std::string& filename);
		void LoadPlugins(const std::string& directoryName);

		void RegisterFunction(const std::string& name, const ExternalFunctionType& function);

#if 0
		void Serialize(mana_stream* stream);
		void Deserialize(mana_stream* stream);
#endif

		void LoadProgram(const std::string& path);
		void LoadProgram(const std::shared_ptr<const void>& program);
		void UnloadProgram();
				
		void Restart();
		bool Run();
		[[nodiscard]] bool IsRunning() const;

		void Execute(const std::function<void()>& function);

		void RequestAll(const int32_t level, const char* actionName, const std::shared_ptr<Actor>& sender) const;
		bool Request(const int32_t level, const char* actorName, const char* actionName, const std::shared_ptr<Actor>& sender);
		void YieldAll();
		[[nodiscard]] const std::shared_ptr<Actor>& GetActor(const char* name);
		[[nodiscard]] const std::string_view& GetActorName(const std::shared_ptr<Actor>& actor) const;
		[[nodiscard]] std::shared_ptr<Actor> CloneActor(const std::shared_ptr<Actor>& actor, const char* newName);
		[[nodiscard]] std::shared_ptr<Actor> CreateActor(const char* name, const char* newName);
		[[nodiscard]] std::shared_ptr<Actor> CreateActorFromPhantom(const char* name, const char* newName);
		[[nodiscard]] bool IsInInitAction() const;
		[[nodiscard]] bool IsFinishInitAction() const;
		
		void SetSystemRequest(const bool enable);
		[[nodiscard]] bool IsSystemRequestEnabled() const;
		
		[[nodiscard]] uint32_t GetFrameCounter() const;
		[[nodiscard]] float_t GetDeltaTime() const;
		[[nodiscard]] bool IsFrameChanged() const;
		
	private:
		[[nodiscard]] int8_t GetInt8FromMemory(const uint32_t address) const;
		[[nodiscard]] uint8_t GetUint8FromMemory(const uint32_t address) const;
		[[nodiscard]] int16_t GetInt16FromMemory(const uint32_t address) const;
		[[nodiscard]] uint16_t GetUint16FromMemory(const uint32_t address) const;
		[[nodiscard]] int32_t GetInt32FromMemory(const uint32_t address) const;
		[[nodiscard]] uint32_t GetUint32FromMemory(const uint32_t address) const;
		[[nodiscard]] float GetFloatFromMemory(const uint32_t address) const;
		[[nodiscard]] const char* GetStringFromMemory(const uint32_t address) const;
		//[[nodiscard]] char* GetStringFromData(const uint32_t address);
		[[nodiscard]] int32_t GetOpecode(const uint32_t address) const;

	private:
		[[nodiscard]] ExternalFunctionType FindFunction(const std::string& functionName) const;
		[[nodiscard]] Buffer& GetGlobalVariables() noexcept;
		[[nodiscard]] const Buffer& GetGlobalVariables() const noexcept;
		[[nodiscard]] Buffer& GetStaticVariables() noexcept;
		[[nodiscard]] const Buffer& GetStaticVariables() const noexcept;

	private:
		std::shared_ptr<Plugin> mPlugin;
		std::shared_ptr<const void> mProgram;
		const FileHeader* mFileHeader = nullptr;
		const uint8_t* mInstructionPool = nullptr;
		const char* mConstantPool = nullptr;
		//std::vector<,*> m,Array;					//!< , オブジェクトへの配列
		std::unordered_map<std::string, ExternalFunctionType> mFunctionHash;
		std::unordered_map<std::string_view, std::shared_ptr<Actor>> mActorHash;		//!< , オブジェクトへの連想配列
		std::unordered_map<std::string_view, const ActorInfoHeader*> mPhantomHash;		//!< phantomを表すmana_actor オブジェクトへの連想配列
		Buffer mGlobalVariables;
		Buffer mStaticVariables;
		
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
