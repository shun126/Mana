/*!
mana (compiler)

@file	Main.cpp
@brief	コマンドラインインターフェース
@detail	このファイルはコンパイラのコマンドラインインターフェースです。
		コンパイル処理そのものは Compiler.h の Compile() が行います。
@author	Shun Moriya
@date	2003-
*/

#include "../runner/common/Setup.h"
#include "../compiler/Compiler.h"
#include "../compiler/Path.h"
#include "../compiler/Version.h"

#include "../runner/Mana.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace
{
	//! コマンドラインの解析結果
	struct CommandLine final
	{
		mana::CompileOptions mOptions;

		//! 出力するプログラムイメージのファイル名。空ならばコンパイル後に実行します
		std::string mOutputFilename;

		//! 出力するC++型宣言ヘッダーのファイル名
		std::string mPublicTypeDeclFilename;

		//! 出力するダンプのファイル名
		std::string mDumpFilename;

		//! 真ならば入力をプログラムイメージとして実行します
		bool mExecute = false;
	};
}

namespace mana
{
	namespace
	{
		/*!
		バッファをファイルへ保存します
		*/
		[[nodiscard]] bool SaveFile(const std::string& filename, const std::string& content)
		{
			std::ofstream file(filename, std::ios::out);
			if (!file.is_open())
			{
				std::cerr << "'" << filename << "' open failed.\n";
				return false;
			}
			file << content;
			return true;
		}

		/*!
		プログラムイメージをファイルへ保存します
		*/
		[[nodiscard]] bool SaveProgramImage(const std::string& filename, const std::vector<uint8_t>& programImage)
		{
			std::ofstream file(filename, std::ios::out | std::ios::binary);
			if (!file.is_open())
			{
				std::cerr << "'" << filename << "' open failed.\n";
				return false;
			}
			file.write(reinterpret_cast<const char*>(programImage.data()), static_cast<std::streamsize>(programImage.size()));
			return true;
		}

		/*!
		プログラムイメージを実行します
		*/
		int Execute(const std::shared_ptr<const void>& program)
		{
			std::shared_ptr<VM> vm = std::make_shared<VM>();
			vm->LoadPlugins(".");
			vm->LoadProgram(program);
			while (vm->Run())
				;
			return 0;
		}

		/*!
		プログラムイメージファイルを実行します
		*/
		int Execute(const std::string& path)
		{
			std::shared_ptr<VM> vm = std::make_shared<VM>();
			vm->LoadPlugins(".");
			vm->LoadProgram(path);
			while (vm->Run())
				;
			return 0;
		}

		/*!
		コンパイルして結果をファイルへ出力します
		*/
		int Generate(const CommandLine& commandLine)
		{
			CompileOptions options = commandLine.mOptions;

			// 診断は発生した順に表示します
			options.mDiagnosticHandler = [](const Diagnostic& diagnostic)
			{
				Trace({ diagnostic.ToString(), "\n" });
			};

			const CompileResult result = Compile(options);

			if (!commandLine.mDumpFilename.empty() && !result.mDump.empty())
			{
				if (!SaveFile(commandLine.mDumpFilename, result.mDump))
					return 1;
			}

			if (!result.mSucceeded)
			{
				if (!commandLine.mOutputFilename.empty())
				{
					std::remove(commandLine.mOutputFilename.c_str());
				}
				return 1;
			}

			if (!commandLine.mPublicTypeDeclFilename.empty())
			{
				if (!SaveFile(commandLine.mPublicTypeDeclFilename, result.mPublicTypeDecl))
					return 1;
			}

			if (commandLine.mOutputFilename.empty())
			{
				// 出力先の指定が無い場合はそのまま実行します
				const auto image = std::make_shared<std::vector<uint8_t>>(result.mProgramImage);
				return Execute(std::shared_ptr<const void>(image, image->data()));
			}

			return SaveProgramImage(commandLine.mOutputFilename, result.mProgramImage) ? 0 : 1;
		}

		void PrintTitle()
		{
			printf("mana %s.%s (%04d-%02d-%02d)\n",
				mana::build::MajorVersion, mana::build::MinorVersion,
				mana::build::Year, mana::build::Month, mana::build::Day);
		}

		void PrintCopyright()
		{
			printf("mana - Copyright (C) 2002-%04d Shun Moriya\n", mana::build::Year);
		}

		void PrintUsage()
		{
			std::cout << "usage:mana [switch] infile\n";
			std::cout << "            -o filename     specify output binary file name\n";
			std::cout << "            -t dirname      specify program header directory name\n";
			std::cout << "            -I filename     force include file (can be specified multiple times)\n";
			std::cout << "            --execute       read infile as a binary file and execute it\n";
			std::cout << "            --help          print this message\n";
			std::cout << "            --copyright     print copyright holder\n";
			std::cout << "            --version       print the version\n";
			std::cout << "\n";
			std::cout << "Report bugs to https://github.com/shun126/Mana/issues\n";
		}

		/*!
		入力ファイル名の拡張子を差し替えたファイル名を作ります
		*/
		[[nodiscard]] std::string ReplaceExtension(const std::string& sourceFilename, const char* extension)
		{
			char drive[_MAX_DRIVE];
			char dir[_MAX_DIR];
			char fileName[_MAX_FNAME];
			char ext[_MAX_EXT];
			char path[_MAX_PATH];
			splitpath(sourceFilename.c_str(), drive, sizeof(drive), dir, sizeof(dir), fileName, sizeof(fileName), ext, sizeof(ext));
			makepath(path, sizeof(path), drive, dir, fileName, extension);
			return std::string(path);
		}

		bool ParseArguments(int argc, char* argv[], CommandLine& commandLine)
		{
			if (argc < 2)
			{
				std::cerr << "No input files\n";
				return false;
			}

			bool generatePublicTypeDeclPathRequested = false;
			bool generateOutputBinaryPathRequested = false;

			for (int cmdcnt = 1; cmdcnt < argc; cmdcnt++)
			{
				char* cmdptr = argv[cmdcnt];
				if (*cmdptr == '-')
				{
					cmdptr++;
					switch (*cmdptr)
					{
					case 'o':
						if (cmdcnt + 1 < argc && *argv[cmdcnt + 1] != '-')
						{
							cmdcnt++;
							commandLine.mOutputFilename = argv[cmdcnt];
						}
						else
						{
							generateOutputBinaryPathRequested = true;
						}
						break;

					case 't':
						commandLine.mOptions.mGeneratePublicTypeDecl = true;
						if (cmdcnt + 1 < argc && *argv[cmdcnt + 1] != '-')
						{
							cmdcnt++;
							commandLine.mPublicTypeDeclFilename = argv[cmdcnt];
						}
						else
						{
							generatePublicTypeDeclPathRequested = true;
						}
						break;

					case 'I':
					{
						if (cmdcnt + 1 >= argc)
						{
							std::cerr << "missing include file for -I\n";
							return false;
						}
						cmdcnt++;
						commandLine.mOptions.mForcedIncludeFiles.emplace_back(argv[cmdcnt]);
						break;
					}

					case '-':
						cmdptr++;
						if (strcmp(cmdptr, "execute") == 0)
						{
							commandLine.mExecute = true;
							break;
						}
						if (strcmp(cmdptr, "copyright") == 0)
						{
							PrintCopyright();
							return false;
						}
						if (strcmp(cmdptr, "version") == 0)
						{
							PrintTitle();
							return false;
						}
						if (strcmp(cmdptr, "help") == 0)
						{
							PrintUsage();
							return false;
						}
						if (strcmp(cmdptr, "debug") == 0)
						{
							commandLine.mOptions.mGenerateDump = true;
							break;
						}
						if (strcmp(cmdptr, "release") == 0)
						{
							break;
						}

						[[fallthrough]]
						;

					default:
						std::cerr << "unrecognized option\n";
						return false;
					}
				}
				else if (commandLine.mOptions.mSourceFilename.empty())
				{
					commandLine.mOptions.mSourceFilename = cmdptr;
				}
				else
				{
					std::cerr << "unrecognized option\n";
					return false;
				}
			}

			const std::string& sourceFilename = commandLine.mOptions.mSourceFilename;

			if (generatePublicTypeDeclPathRequested)
			{
				commandLine.mPublicTypeDeclFilename = ReplaceExtension(sourceFilename, ".h");
			}
			if (generateOutputBinaryPathRequested)
			{
				commandLine.mOutputFilename = ReplaceExtension(sourceFilename, ".mx");
			}
			if (commandLine.mOptions.mGenerateDump)
			{
				commandLine.mDumpFilename = ReplaceExtension(sourceFilename, ".md");
			}

			return true;
		}
	}
}

int main(int argc, char* argv[])
{
	int result = 0;

#if MANA_TARGET_WINDOWS && MANA_BUILD_TARGET < MANA_BUILD_RELEASE
	_CrtMemState stOldMemState;
	_CrtMemState stNewMemState;
	_CrtMemState stDiffMemState;
	_CrtMemCheckpoint(&stOldMemState);

	/*
	 * メモリリーク番号が分かる場合は
	 * _CrtSetBreakAlloc(メモリリーク番号);
	 * で指定すると調べる事ができます
	 */
	//_CrtSetBreakAlloc(423);
#endif

	{
		CommandLine commandLine;
		if (mana::ParseArguments(argc, argv, commandLine))
		{
			if (commandLine.mExecute)
				result = mana::Execute(commandLine.mOptions.mSourceFilename);
			else
				result = mana::Generate(commandLine);
		}
	}

#if MANA_TARGET_WINDOWS && MANA_BUILD_TARGET < MANA_BUILD_RELEASE
	_CrtMemCheckpoint(&stNewMemState);
	if (_CrtMemDifference(&stDiffMemState, &stOldMemState, &stNewMemState))
	{
		_RPT0(_CRT_WARN, "Memory leaked!\n\n");
		_CrtMemDumpStatistics(&stDiffMemState);
		_CrtDumpMemoryLeaks();
	}
#endif

	return result;
}
