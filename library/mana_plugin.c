/*!
 * mana (library)
 *
 * @file	mana_plugin.c
 * @brief	�v���O�C���֌W�̃\�[�X�t�@�C��
 * @detail	���̃t�@�C���̓v���O�C���֌W�̃\�[�X�t�@�C���ł��B
 * @author	Shun Moriya
 * @date	2003-
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
/*! �_�C�i�~�b�N���C�u�����̃n���h�� */
#define MODULE				HMODULE
/*! �_�C�i�~�b�N���C�u������ǂݍ��� */
#define LOAD_LIBRARY(N)		LoadLibrary(N);
/*! �_�C�i�~�b�N���C�u�������J�� */
#define FREE_LIBRARY(M)		FreeLibrary(M);
/*! �_�C�i�~�b�N���C�u�������̊֐����擾 */
#define GET_PROC_ADR(M, N)	GetProcAddress(M, N);
#elif defined(NN_PLATFORM_CTR)
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dlfcn.h>
/*! �_�C�i�~�b�N���C�u�����̃|�C���^ */
#define MODULE				void*
/*! �_�C�i�~�b�N���C�u������ǂݍ��� */
#define LOAD_LIBRARY(N)		dlopen(N, RTLD_LAZY);
/*! �_�C�i�~�b�N���C�u�������J�� */
#define FREE_LIBRARY(M)		dlclose(M);
/*! �_�C�i�~�b�N���C�u�������̊֐����擾 */
#define GET_PROC_ADR(M, N)	dlsym(M, N);
#endif

#if !defined(NN_PLATFORM_CTR)
static MODULE* mana_plugins = NULL;
static uint32_t mana_plugin_count = 0;

/*!
 * void mana_initialize(void)����Ă΂��̂ŁA�Ăяo���K�v�͂���܂���B
 */
void mana_plugin_initialize(void)
{
	mana_plugin_finalize();

	mana_plugins = NULL;
	mana_plugin_count = 0;
}

/*!
 * void mana_finalize(void)����Ă΂��̂ŁA�Ăяo���K�v�͂���܂���B
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
 * @param[in]	file_name	�v���O�C���̃t�@�C����
 * @retval		true		����
 * @retval		false		���s
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
 * @param[in]	directory_name	�f�B���N�g����
 * @retval		true			����
 * @retval		false			���s
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
#endif
