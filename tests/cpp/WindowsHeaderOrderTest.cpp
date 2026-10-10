/*!
 * mana (test)
 *
 * @file	WindowsHeaderOrderTest.cpp
 * @brief	windows.h を Mana より先に読み込む場合の試験
 * @detail	ホストが windows.h を先に読み込んでも、Mana の Win32 宣言が
 * 		windows.h の宣言と衝突しないことを試験します。
 * @author	Shun Moriya
 * @date	2026-
 */

#define NOMINMAX
#include <windows.h>

#include "../../runner/Mana.h"

bool LoadKernel32WithWindowsHeaderFirst()
{
	const HMODULE module = LoadLibraryA("kernel32.dll");
	if (module == nullptr)
		return false;

	// Plugin stores the handle as mana::windows::ModuleHandle.
	const mana::windows::ModuleHandle handle = module;
	const bool found = GetProcAddress(handle, "GetTickCount") != nullptr;
	FreeLibrary(handle);
	return found;
}
