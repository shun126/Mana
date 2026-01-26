/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "Main.h"
#include "CodeGenerator.h"
#include "ErrorHandler.h"
#include "Lexer.h"
#include "Path.h"
#include "ParsingDriver.h"
#include "Version.h"

#include "../runner/Mana.h"

#include <ctime>

namespace mana
{
	char mInputFilename[_MAX_PATH];
	char mOutputFilename[_MAX_PATH];
	bool mExecute;
	bool mDebug;
	bool mRelease;

	std::ofstream mVariableHeaderFile;

	const char* GetInputFilename()
	{
		return mInputFilename;
	}

	const char* GetOutputFilename()
	{
		return mOutputFilename;
	}

	const char* GetTargetFilename()
	{
		return mOutputFilename[0] != '\0' ? mOutputFilename : mInputFilename;
	}

	/*
	SystemHolder& GetSystemHolder()
	{
		return mana::mSystemHolder;
	}
	*/

	static void Dump(const std::shared_ptr<ParsingDriver>& parser)
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fileName[_MAX_FNAME];
		char ext[_MAX_EXT];
		char path[_MAX_PATH];
		splitpath(mInputFilename, drive, sizeof(drive), dir, sizeof(dir), fileName, sizeof(fileName), ext, sizeof(ext));
		makepath(path, sizeof(path), drive, dir, fileName, ".md");

		std::ofstream log(path);
		if (log.is_open())
		{
			if (parser->GetSymbolFactory())
			{
				log << "# Symbol Table\n";
				parser->GetSymbolFactory()->Dump(log);
				log << "\n";
			}
			if (parser->GetRootSyntaxNode())
			{
				log << "# Syntax Node\n";
				parser->GetRootSyntaxNode()->Dump(log);
			}
			if (parser->GetCodeGenerator())
			{
				log << "# Code\n";
				log << "```\n";
				parser->GetCodeGenerator()->Dump(log);
				log << "```\n";
			}
		}
	}

	static bool GenerateBinary(OutputStream& stream, const std::shared_ptr<ParsingDriver>& parser)
	{
		FileHeader header = {};
		memcpy(&header.mHeader, Signature, sizeof(header.mHeader));
		header.mMajorVersion = MajorVersion;
		header.mMinorVersion = MinorVersion;

		// TODO バイナリファイルにアセットデータを結合する必要があるか検討して下さい
#if 0
		if (GetSize() > 0)
			header.mFlag |= (1 << FileHeader::Flag::Resource);
#endif
		if (IsBigEndian())
			header.mFlag |= (1 << FileHeader::Flag::BigEndian);
#if UINTPTR_MAX == UINT64_MAX
		header.mFlag |= (1 << FileHeader::Flag::Is64Bit);
#endif
		const size_t numberOfActors = parser->GetSymbolFactory()->GetNumberOfActors();
		if (std::numeric_limits<uint32_t>::max() < numberOfActors)
			throw std::overflow_error("Too many actors defined");
		header.mNumberOfActors = static_cast<uint32_t>(numberOfActors);
		header.mSizeOfConstantPool = parser->GetDataBuffer()->GetSize();
		header.mSizeOfInstructionPool = parser->GetCodeBuffer()->GetSize();
		header.mSizeOfStaticMemory = parser->GetSymbolFactory()->GetStaticMemoryAddress();
		header.mSizeOfGlobalMemory = parser->GetSymbolFactory()->GetGlobalMemoryAddress();
		header.mRandomSeedNumber = static_cast<uint32_t>(time(nullptr));

		stream.PushData(&header, sizeof(header));

		if (!parser->GetSymbolFactory()->GenerateActorInformation(stream))
		{
			return false;
		}

		parser->GetDataBuffer()->Write(stream);
		parser->GetCodeBuffer()->Write(stream);

		// TODO バイナリファイルにアセットデータを結合する必要があるか検討して下さい
#if 0
		if (GetSize() > 0)
		{
			size_t position = mana_stream_get_used_size(stream);
			if (position > 0)
			{
				srand((unsigned)time(NULL));
				for (size_t i = 0; i < position; i++)
				{
					mana_stream_push_unsigned_char(stream, (const uint8_t)rand());
			}
		}
			if (!datalink_generator_write_data(stream))
			{
				result = 1;
				goto ESCAPE;
			}
			}
#endif

		 return true;
	}

	static bool Execute(const std::shared_ptr<const void>& program)
	{
		std::shared_ptr<VM> vm = std::make_shared<VM>();
		vm->LoadPlugins(".");
		vm->LoadProgram(program);
		while (vm->Run())
			;
		return true;
	}

	int Execute(const std::string& path)
	{
		std::shared_ptr<VM> vm = std::make_shared<VM>();
		vm->LoadPlugins(".");
		vm->LoadProgram(path);
		while (vm->Run())
			;
		return 0;
	}

	int Generate()
	{
		int result = 0;

		try
		{
			std::shared_ptr<ParsingDriver> parser = std::make_shared<ParsingDriver>();
			if (parser == nullptr)
				throw std::bad_alloc();

			if (lexer::Initialize(parser, mInputFilename))
			{
				// TODO スクリプトのグローバル変数を構造体としてヘッダーに出力する必要があるか検討して下さい
#if 0
				if (mVariableHeaderFile)
				{
					fprintf(mVariableHeaderFile, "#pragma once\n");
					fprintf(mVariableHeaderFile, "typedef struct mana_global\n{\n");
				}
#endif

				result = parser->Parse() != 0 || yynerrs != 0;

				// TODO スクリプトのグローバル変数を構造体としてヘッダーに出力する必要があるか検討して下さい
#if 0
				if (mVariableHeaderFile)
				{
					fprintf(mVariableHeaderFile, "}mana_global;\n");
					fprintf(mVariableHeaderFile, "#endif\n");
					fclose(mVariableHeaderFile);
					mVariableHeaderFile = NULL;
				}
#endif

				if (mDebug)
				{
					Dump(parser);
				}

				parser->GetSymbolFactory()->CheckUndefine();

				if (result == 0)
				{
					OutputStream stream;
					if (GenerateBinary(stream, parser))
					{
						if (mOutputFilename[0] != '\0')
						{
							stream.Save(mOutputFilename);
						}
						else
						{
							Execute(stream.MakeShared());
						}
					}
				}
				else
				{
					if (mOutputFilename[0] != '\0')
					{
						std::remove(mOutputFilename);
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			mana::Fatal(e.what());
		}

		lexer::Finalize();

		return result;
	}

	static void PrintTitle()
	{
		printf("mana %s.%s (%04d-%02d-%02d)\n",
			mana::build::MajorVersion, mana::build::MinorVersion,
			mana::build::Year, mana::build::Month, mana::build::Day);
	}

	static void PrintCopyright()
	{
		printf("mana - Copyright (C) 2002-%04d Shun Moriya\n", mana::build::Year);
	}

	static void PrintUsage()
	{
		std::cout << "usage:mana [switch] infile\n";
		std::cout << "            -o filename     specify output binary file name\n";
		std::cout << "            -i dirname      specify program header directory name\n";
		std::cout << "            --execute       read infile as a binary file and execute it\n";
		std::cout << "            --help          print this message\n";
		std::cout << "            --copyright     print copyright holder\n";
		std::cout << "            --version       print the version\n";
		std::cout << "\n";
		std::cout << "Report bugs to https://github.com/shun126/Mana/issues\n";
	}

	static bool ParseArguments(int argc, char* argv[])
	{
		if (argc < 2)
		{
			std::cerr << "No input files\n";
			return false;
		}
		else
		{
			std::string globalTypeHeaderDirectoryName;
			bool outputBinary = false;

			mInputFilename[0] = '\0';
			mOutputFilename[0] = '\0';
			mExecute = false;
			mDebug = false;
			mRelease = false;

			for (int cmdcnt = 1; cmdcnt < argc; cmdcnt++)
			{
				char* cmdptr = argv[cmdcnt];
				if (*cmdptr == '-')
				{
					cmdptr++;
					switch (*cmdptr)
					{
					case 'o':
						cmdcnt++;
						if (cmdcnt >= argc)
							outputBinary = true;
						else
							strcpy(mOutputFilename, sizeof(mOutputFilename), argv[cmdcnt]);
						break;

					case 'i':
					{
						char filename[_MAX_PATH];
						if (cmdcnt + 1 < argc && *argv[cmdcnt + 1] != '-')
						{
							cmdcnt++;
							strcpy(filename, sizeof(filename), argv[cmdcnt]);
						}
						else
						{
							filename[0] = '\0';
						}
						const size_t length = strlen(filename);
						if (length > 0 && filename[length - 1] != PathSeparator())
						{
							filename[length] = PathSeparator();
							filename[length + 1] = '\0';
						}
						globalTypeHeaderDirectoryName = filename;

						break;
					}

					case 'e':
						mExecute = true;
						break;

					case '-':
						cmdptr++;
						if (strcmp(cmdptr, "execute") == 0)
						{
							mExecute = true;
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
							mDebug = true;
							break;
						}
						if (strcmp(cmdptr, "release") == 0)
						{
							mRelease = true;
							break;
						}
						//[[fallthrough]]
					default:
						std::cerr << "unrecognized option\n";
						return false;
					}
				}
				else if (mInputFilename[0] == '\0')
				{
					strcpy(mInputFilename, sizeof(mInputFilename), cmdptr);
				}
				else
				{
					std::cerr << "unrecognized option\n";
					return false;
				}
			}

			// check
			{
				char drive[_MAX_DRIVE];
				char dir[_MAX_DIR];
				char fileName[_MAX_FNAME];
				char ext[_MAX_EXT];
				splitpath(mInputFilename, drive, sizeof(drive), dir, sizeof(dir), fileName, sizeof(fileName), ext, sizeof(ext));

				// output Binary
				if (outputBinary)
				{
					makepath(mOutputFilename, sizeof(mOutputFilename), drive, dir, fileName, ".mx");
				}

				// output global types
				if (!globalTypeHeaderDirectoryName.empty())
				{
					globalTypeHeaderDirectoryName += fileName;
					globalTypeHeaderDirectoryName += ".h";

					mVariableHeaderFile.open(globalTypeHeaderDirectoryName, std::ios::out);
					if (!mVariableHeaderFile.is_open())
					{
						std::cerr << "'" << globalTypeHeaderDirectoryName << "' open failed.\n";
						return false;
					}
				}
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
	/// _CrtSetBreakAlloc(363);
#endif

	if (mana::ParseArguments(argc, argv))
	{
		if (mana::mExecute)
			result = mana::Execute(mana::mInputFilename);
		else
			result = mana::Generate();
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
