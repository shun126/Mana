/*!
mana (library)

@file	mana_plugin.c
@brief	プラグイン関係のソースファイル
@detail	このファイルはプラグイン関係のソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_DEFINE_H___)
#include "mana_define.h"
#endif
#if !defined(___MANA_MALLOC_H___)
#include "mana_malloc.h"
#endif
#if !defined(___MANA_PLUGIN_H___)
#include "mana_plugin.h"
#endif
#include <assert.h>
#include <string.h>

#if defined(_MSC_VER)
#include <windows.h>
/*! ダイナミックライブラリのハンドル */
#define MODULE				HMODULE
/*! ダイナミックライブラリを読み込む */
#define LOAD_LIBRARY(N)		LoadLibrary(N);
/*! ダイナミックライブラリを開放 */
#define FREE_LIBRARY(M)		FreeLibrary(M);
/*! ダイナミックライブラリ内の関数を取得 */
#define GET_PROC_ADR(M, N)	GetProcAddress(M, N);
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dlfcn.h>
/*! ダイナミックライブラリのポインタ */
#define MODULE				void*
/*! ダイナミックライブラリを読み込む */
#define LOAD_LIBRARY(N)		dlopen(N, RTLD_LAZY);
/*! ダイナミックライブラリを開放 */
#define FREE_LIBRARY(M)		dlclose(M);
/*! ダイナミックライブラリ内の関数を取得 */
#define GET_PROC_ADR(M, N)	dlsym(M, N);
#endif

static MODULE* mana_plugins = NULL;
static uint32_t mana_plugin_count = 0;

/*!
 * void mana_initialize(void)から呼ばれるので、呼び出す必要はありません。
 */
void mana_plugin_initialize(void)
{
	mana_plugin_finalize();

	mana_plugins = NULL;
	mana_plugin_count = 0;
}

/*!
 * void mana_finalize(void)から呼ばれるので、呼び出す必要はありません。
 */
void mana_plugin_finalize(void)
{
	if(mana_plugins)
	{
		uint32_t i;

		for(i = 0; i < mana_plugin_count; i++)
		{
			MODULE module;

			module = mana_plugins[i];
			if(module)
			{
				typedef int32_t (*mana_finalize)(void);

				mana_finalize function = (mana_finalize)GET_PROC_ADR(module, "mana_finalize");
				if(function)
					function();

				FREE_LIBRARY(module);
			}
		}

		mana_free(mana_plugins);
		mana_plugins = NULL;
		mana_plugin_count = 0;
	}
}

/*!
 * @param[in]	file_name	プラグインのファイル名
 * @retval		true		成功
 * @retval		false		失敗
 */
bool mana_plugin_load(const char* file_name)
{
	MODULE module = LOAD_LIBRARY(file_name);
	if(module)
	{
		typedef int32_t (*mana_initialize)(void);

		mana_initialize function = (mana_initialize)GET_PROC_ADR(module, "mana_initialize");
		if(function && function())
		{
			mana_plugins = mana_realloc(mana_plugins, mana_plugin_count + 1);
			mana_plugins[mana_plugin_count] = module;
			mana_plugin_count++;
			return true;
		}

		FREE_LIBRARY(module);
	}

	return false;
}

/*!
 * @param[in]	directory_name	ディレクトリ名
 * @retval		true			成功
 * @retval		false			失敗
 */
bool mana_plugin_regist(const char* directory_name)
{
#if defined(_MSC_VER)
	{
		HANDLE handle;
		WIN32_FIND_DATA fd;
		int8_t entry[_MAX_PATH];

#if __STDC_WANT_SECURE_LIB__
		strcpy_s(entry, sizeof(entry), directory_name);
		strcat_s(entry, sizeof(entry), "\\*.ml");
#else
		strcpy(entry, directory_name);
		strcat(entry, "\\*.ml");
#endif

		handle = FindFirstFile(entry, &fd);
		if(handle != INVALID_HANDLE_VALUE)
		{
			do{
				if(strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
				{
					if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						if(!mana_plugin_regist(fd.cFileName))
							return false;
					}
					else
					{
						if(!mana_plugin_load(fd.cFileName))
							return false;
					}
				}
			}
			while(FindNextFile(handle, &fd));
		}

		FindClose(handle);
	}
#else
	{
		DIR* dir = opendir(directory_name);
		if(dir)
		{
			struct dirent* entry;

			while((entry = readdir(dir)) != NULL)
			{
				if(strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
				{
					struct stat fi;

					stat(entry->d_name,&fi);
					if(S_ISDIR(fi.st_mode))
					{
						if(!mana_plugin_regist(entry->d_name))
							return false;
					}
					else
					{
						char* position = strrchr(entry->d_name, '.');
						if(position&& strcmp(position, ".ml") == 0)
						{
							if(!mana_plugin_load(entry->d_name))
								return false;
						}
					}
				}
			}

			closedir(dir);
		}
	}
#endif

	return true;
}
