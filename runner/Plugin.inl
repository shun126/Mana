/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "common/Platform.h"

#if defined(MANA_TARGET_WINDOWS)

#define NOMINMAX
#include <windows.h>
/*! ダイナミックライブラリのハンドル */
//#define MODULE				HMODULE
/*! ダイナミックライブラリを読み込む */
#define LOAD_LIBRARY(N)		LoadLibrary(N)
/*! ダイナミックライブラリを開放 */
#define FREE_LIBRARY(M)		FreeLibrary(M)
/*! ダイナミックライブラリ内の関数を取得 */
#define GET_PROC_ADR(M, N)	GetProcAddress(M, N)

#else

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dlfcn.h>
#include <unistd.h>
/*! ダイナミックライブラリを読み込む */
#define LOAD_LIBRARY(N)		dlopen(N, RTLD_LAZY)
/*! ダイナミックライブラリを開放 */
#define FREE_LIBRARY(M)		dlclose(M)
/*! ダイナミックライブラリ内の関数を取得 */
#define GET_PROC_ADR(M, N)	dlsym(M, N)

#endif

namespace mana
{
	inline Plugin::Plugin(const std::shared_ptr<VM>& vm)
		: mVM(vm)
	{
	}

	inline Plugin::~Plugin()
	{
		auto vm = mVM.lock();
		if (vm == nullptr)
		{
			return;
		}

		for (auto plugin : mPlugins)
		{
			using Finalize = int32_t(*)(const std::shared_ptr<VM>&);
			if (const auto function = reinterpret_cast<Finalize>(GET_PROC_ADR(plugin, "Finalize")))
				function(vm);
			FREE_LIBRARY(plugin);
		}
	}

	inline void Plugin::Load(const std::string& fileName)
	{
		const auto vm = mVM.lock();
		if (vm == nullptr)
		{
			return;
		}

		const MODULE module = LOAD_LIBRARY(fileName.c_str());
		if (module == nullptr)
		{
			return;
		}

		using Initialize = int32_t(*)(const std::shared_ptr<VM>&);
		const auto function = reinterpret_cast<Initialize>(GET_PROC_ADR(module, "Initialize"));
		if (function && function(vm))
		{
			mPlugins.push_back(module);
			return;
		}
		FREE_LIBRARY(module);
	}

	inline void Plugin::Register(const std::string& directoryName)
	{
#if defined(MANA_TARGET_WINDOWS)
		{
			char entry[_MAX_PATH];

			strcpy_s(entry, _MAX_PATH, directoryName.c_str());
			strcat_s(entry, _MAX_PATH, "\\*.ml");

			WIN32_FIND_DATA fd;
			const HANDLE handle = FindFirstFile(entry, &fd);
			if (handle != INVALID_HANDLE_VALUE)
			{
				do {
					if (strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
					{
						if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						{
							Register(fd.cFileName);
						}
						else
						{
							Load(fd.cFileName);
						}
					}
				} while (FindNextFile(handle, &fd));
			}

			FindClose(handle);
		}
#else
		{
			DIR* dir = opendir(directoryName.c_str());
			if(dir)
			{
				struct dirent* entry;

				while((entry = readdir(dir)) != nullptr)
				{
					if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
					{
						struct stat fi;
						stat(entry->d_name, &fi);
						if(S_ISDIR(fi.st_mode))
						{
							Register(entry->d_name);
						}
						else
						{
							char* position = strrchr(entry->d_name, '.');
							if(position&& strcmp(position, ".ml") == 0)
							{
								Load(entry->d_name);
							}
						}
					}
				}

				closedir(dir);
			}
		}
#endif
	}
}
