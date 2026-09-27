/*!
mana (compiler/library)

@author	Shun Moriya
@date	2026-
*/

#pragma once
#include "Platform.h"

#if defined(MANA_TARGET_WINDOWS)
#include <cstdint>

// Declares the few Win32 functions Mana calls instead of including windows.h,
// whose macros (GetObject, min, max, ...) break host headers such as those of
// Unreal Engine. The declarations match windows.h, so the host may include
// windows.h before or after Mana. As with windows.h, NO_STRICT must be defined
// before this header when the host uses it.
#if !defined(NO_STRICT)
struct HINSTANCE__;
#endif

namespace mana::windows
{
#if defined(NO_STRICT)
	using ModuleHandle = void*;
#else
	using ModuleHandle = HINSTANCE__*;
#endif
	using ProcAddress = intptr_t(__stdcall*)();
}

extern "C"
{
	__declspec(dllimport) mana::windows::ModuleHandle __stdcall LoadLibraryA(const char* fileName);
	__declspec(dllimport) mana::windows::ProcAddress __stdcall GetProcAddress(mana::windows::ModuleHandle module, const char* procName);
	__declspec(dllimport) int __stdcall FreeLibrary(mana::windows::ModuleHandle module);
	__declspec(dllimport) void __stdcall OutputDebugStringA(const char* outputString);
}
#endif
