/*!
mana (compiler/library)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "Platform.h"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>

#if defined(MANA_TARGET_WINDOWS)
#define NOMINMAX
#include <windows.h>
#else
#include <cstring>
#endif

namespace mana
{
	[[nodiscard]] inline std::string Concat(const std::initializer_list<std::string_view>& r)
	{
		std::size_t n = 0;
		for (auto& it : r)
			n += it.size();
		std::string l;
		l.reserve(n);
		for (auto& it : r)
			l += it;
		return l;
	}

	//! Traceの重大度
	enum class TraceLevel : uint8_t
	{
		Info,		//!< print()の出力や実行トレース
		Warning,	//!< 警告
		Error		//!< エラー、アサート、内部矛盾
	};

	/*!
	Traceの出力先

	@param[in]	userData	SetTraceHandlerに渡した値
	@param[in]	level		重大度
	@param[in]	message		UTF-8のNUL終端文字列
	@param[in]	length		NUL終端を含まない長さ
	*/
	using TraceHandler = void(*)(void* userData, const TraceLevel level, const char* message, const std::size_t length);

	struct TraceSink final
	{
		TraceHandler mHandler = nullptr;
		void* mUserData = nullptr;
	};

	//! 現在の出力先を保持します
	[[nodiscard]] inline TraceSink& GetTraceSink() noexcept
	{
		static TraceSink sink;
		return sink;
	}

	/*!
	Traceの出力先を差し替えます

	既定では標準出力へ出力します。
	Unreal Engineなど、標準出力が届かない
	環境へ組み込む場合に差し替えて下さい。

	@attention	利用を開始する前に一度だけ設定して下さい。
					出力中の差し替えは安全ではありません。
	@attention	MANA_BUG等は出力の直後に終了するため、
					handlerは例外を送出しないで下さい。
	@attention	handlerは行単位で呼ばれるとは限りません。実行トレースの様に
					1行を複数回に分けて出力する箇所があるため、行単位で扱いたい
					場合はhandler側で改行まで蓄えて下さい。

	@param[in]	handler		出力先。nullptrで既定に戻ります
	@param[in]	userData	handlerへそのまま渡される値
	*/
	inline void SetTraceHandler(const TraceHandler handler, void* userData = nullptr) noexcept
	{
		TraceSink& sink = GetTraceSink();
		sink.mHandler = handler;
		sink.mUserData = userData;
	}

	inline void Trace(const TraceLevel level, const std::string& message)
	{
		const TraceSink& sink = GetTraceSink();
		if (sink.mHandler != nullptr)
		{
			sink.mHandler(sink.mUserData, level, message.c_str(), message.size());
			return;
		}

#if defined(MANA_TARGET_WINDOWS) && (MANA_BUILD_TARGET == MANA_BUILD_DEBUG)
		OutputDebugStringA(message.c_str());
#endif
		std::cout << message;

		// アサートは出力の直後に終了するため、
		// バッファに溜まったまま失われないよう書き出します
		if (level != TraceLevel::Info)
		{
			std::cout.flush();
		}
	}

	inline void Trace(const TraceLevel level, const std::initializer_list<std::string_view> message)
	{
		Trace(level, Concat(message));
	}

	inline void Trace(const std::string& message)
	{
		Trace(TraceLevel::Info, message);
	}

	inline void Trace(const std::initializer_list<std::string_view> message)
	{
		Trace(TraceLevel::Info, Concat(message));
	}

	inline void strcpy(char* outBuffer, size_t outBufferSize, const char* inBuffer)
	{
#if defined(MANA_TARGET_WINDOWS)
		strcpy_s(outBuffer, outBufferSize, inBuffer);
#else
		std::strncpy(outBuffer, inBuffer, outBufferSize);
#endif
	}

	inline void strcat(char* outBuffer, size_t outBufferSize, const char* inBuffer)
	{
#if defined(MANA_TARGET_WINDOWS)
		strcat_s(outBuffer, outBufferSize, inBuffer);
#else
		std::strncat(outBuffer, inBuffer, outBufferSize);
#endif
	}

	inline int strcmp(char const* buffer1, char const* buffer2)
	{
		return std::strcmp(buffer1, buffer2);
	}
}

//! コンソールに文字列を出力
#define MANA_PRINT(...)		mana::Trace(__VA_ARGS__)
#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
//! コンソールに文字列を出力(デバッグビルドのみ)
#define MANA_TRACE(...)		mana::Trace(__VA_ARGS__)
#else
//! コンソールに文字列を出力(デバッグビルドのみ)
#define MANA_TRACE(...)		((void)0)
#endif
