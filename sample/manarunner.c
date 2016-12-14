/*!
 * mana (runner)
 *
 * @file	manarunner.c
 * @brief	メインループに関するヘッダーファイル
 * @detail	このファイルはオプションなどメインループに関係するヘッダーファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#include "manarunner.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32) || defined(_WIN64)
#define chdir _chdir
#include <windows.h>
#include <GL/glut.h>
#include <direct.h>
#else
#include <GLUT/glut.h>
#include <unistd.h>
#endif

static char input_filename[FILENAME_MAX];

static void idle(void)
{
	glutPostRedisplay();
}

static bool parse_arguments(int argc, char* argv[])
{
	if(argc == 1)
	{
		return false;
	}

#if defined(__STDC_WANT_SECURE_LIB__)
	strcpy_s(input_filename, sizeof(input_filename), "startup.mx");
#else
	strcpy(input_filename, "startup.mx");
#endif

	for(int cmdcnt = 1; cmdcnt < argc; cmdcnt++)
	{
		char* cmdptr = argv[cmdcnt];
		if(*cmdptr == '-')
		{
			cmdptr++;
			switch(*cmdptr)
			{
			case 'C':
				if(++cmdcnt >= argc)
				{
					printf("Can't change directory\n");
					return false;
				}
				if(chdir(argv[cmdcnt]) != 0)
				{
					printf("Can't change directory to %s\n", argv[cmdcnt]);
					return false;
				}
				break;

			case '-':
				cmdptr ++;
				if(strcmp(cmdptr, "copyright") == 0)
				{
					printf("Mana Runner - Copyright (C) 2009 Shun Moriya.\n");
					return false;
				}
				else if(strcmp(cmdptr, "version") == 0)
				{
					printf("Mana Runner 0.0.2\n");
					return false;
				}
				else if(strcmp(cmdptr, "help") == 0)
				{
					printf("usage:manarunner [switch] infile\n");
					printf("                  --help          print this message\n");
					printf("                  --copyright     print copyright holder\n");
					printf("                  --version       print the version\n");
					printf("\n\nReport bugs to <shun@mnu.sakura.ne.jp>\n");
					return false;
				}

			default:
				printf("unrecognized option\n");
				return false;
			}
		}
		else
		{
#if defined(__STDC_WANT_SECURE_LIB__)
			strcpy_s(input_filename, sizeof(input_filename), cmdptr);
#else
			strcpy(input_filename, cmdptr);
#endif
		}
	}

	return true;
}

int main(int argc, char* argv[])
{
	if(!parse_arguments(argc, argv))
	{
		return 1;
	}

	FILE* file;
#if defined(__STDC_WANT_SECURE_LIB__)
	if(fopen_s(&file, input_filename, "rb") != 0)
#else
	if((file = fopen(input_filename, "rb")) == NULL)
#endif
	{
		printf("file open failed: %s\n", input_filename);
		return 1;
	}

	if(fseek(file, 0L, SEEK_END) != 0)
	{
		fclose(file);
		printf("file open failed: %s\n", input_filename);
		return 1;
	}

	const long size = ftell(file);

	void* program = malloc(size);
	if(program == NULL)
	{
		fclose(file);
		printf("memory allocation failed\n");
		return 1;
	}

	rewind(file);

	if(fread(program, 1, size, file) != size)
	{
		fclose(file);
		free(program);
		printf("file read failed: %s\n", input_filename);
		return 1;
	}

	fclose(file);

	mana_initialize();

	mana_plugin_regist(".");

	int result = 0;

	glutInit(&argc, argv);

	glutCreateWindow("mana runner");

	glutIdleFunc(idle);

/*
	glutDisplayFunc(display);
	glutReshapeFunc(resize);

	glutKeyboardFunc(keyboard);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);
*/

	{
		mana* mana_instance = mana_create();
		if(mana_instance)
		{
			if(mana_load_program(mana_instance, program, TRUE))
			{
				// TODO:経過時間を計測してください
				while(mana_run(mana_instance, 1.0f / 60.0f))
				{
				}
			}
			else
			{
				result = 1;
			}
			mana_destroy(mana_instance);
		}
		else
		{
			result = 1;
		}
	}


  	glutMainLoop();

	mana_finalize();

	return result;
}
