/*!
mana (test)

@file	WindowsHeaderTest.cpp
@brief	windows.h との共存の試験
@detail	Mana のヘッダーが windows.h のマクロ（GetObject、min、max など）を
		持ち込まないこと、そしてホストが windows.h を Mana の後に読み込んでも
		宣言が衝突しないことを試験します。
		Mana より先に windows.h を読み込む場合は WindowsHeaderOrderTest.cpp で
		試験します。
@author	Shun Moriya
@date	2026-
*/

#include "../../runner/Mana.h"

#if defined(_WINDOWS_) || defined(GetObject) || defined(GetMessage) || defined(min) || defined(max)
#error Mana headers must not include windows.h
#endif

#include <windows.h>

#include <iostream>

// Defined in WindowsHeaderOrderTest.cpp.
bool LoadKernel32WithWindowsHeaderFirst();

int main()
{
	int result = 0;

	const mana::windows::ModuleHandle module = LoadLibraryA("kernel32.dll");
	if (module == nullptr || GetProcAddress(module, "GetTickCount") == nullptr)
	{
		std::cerr << "failed to load kernel32.dll" << std::endl;
		result = 1;
	}
	if (module != nullptr)
		FreeLibrary(module);

	if (!LoadKernel32WithWindowsHeaderFirst())
	{
		std::cerr << "failed to load kernel32.dll with windows.h first" << std::endl;
		result = 1;
	}

	const uint64_t begin = mana::GetMicroSecond();
	Sleep(20);
	const uint64_t end = mana::GetMicroSecond();
	if (end < begin + 10000)
	{
		std::cerr << "GetMicroSecond did not advance: " << begin << " -> " << end << std::endl;
		result = 1;
	}

	return result;
}
