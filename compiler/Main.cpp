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
	std::shared_ptr<ParsingDriver> mParser;
	static char mInputFilename[_MAX_PATH];
	static char mOutputFilename[_MAX_PATH];
	int32_t mDebug;
	int32_t mRelease;

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

	static void Dump()
	{
		char filename[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		splitpath(mInputFilename, drive, sizeof(drive), dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
		makepath(filename, sizeof(filename), drive, dir, fname, ".log");

		std::ofstream log(filename);
		if (log.is_open())
		{
			{
				log << "Symbol Table\n\n";
				mParser->GetSymbolFactory()->Dump(log);
				log << "\n";
			}
			{
				log << "Code\n";
				mParser->GetCodeGenerator()->Dump(log);
				log << "\n";
			}
		}
	}

	static bool GenerateBinary(OutputStream& stream)
	{
		FileHeader header;
		memset(&header, 0, sizeof(FileHeader));
		memcpy(&header.mHeader, SIGNATURE, sizeof(header.mHeader));
		header.mMajorVersion = MAJOR_VERSION;
		header.mMinorVersion = MINOR_VERSION;
		/* TODO:Unimplemented
		if (GetSize() > 0)
			header.mFlag |= (1 << FileHeader::Flag::Resource);
		*/
		if (IsBigEndian())
			header.mFlag |= (1 << FileHeader::Flag::BigEndian);
#if UINTPTR_MAX == UINT64_MAX
		header.mFlag |= (1 << FileHeader::Flag::Is64bit);
#endif
		header.mNumberOfActors = mParser->GetSymbolFactory()->GetNumberOfActors();
		header.mSizeOfConstantPool = mParser->GetDataBuffer()->GetSize();
		header.mSizeOfInstructionPool = mParser->GetCodeBuffer()->GetSize();
		header.mSizeOfStaticMemory = mParser->GetSymbolFactory()->GetStaticMemoryAddress();
		header.mSizeOfGlobalMemory = mParser->GetSymbolFactory()->GetGlobalMemoryAddress();
		header.mRandomSeedNumber = static_cast<uint32_t>(time(NULL));

		stream.PushData(&header, sizeof(header));

		if (!mParser->GetSymbolFactory()->GenerateActorInfomation(stream))
		{
			return false;
		}

		mParser->GetDataBuffer()->Write(stream);
		mParser->GetCodeBuffer()->Write(stream);
#if 0
		TODO:Unimplemented

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

	static bool Execute(const std::shared_ptr<const void> program)
	{
		std::shared_ptr<VM> vm = std::make_shared<VM>();
		vm->LoadPlugins(".");
		vm->LoadProgram(program);
		while (vm->Run())
			;
		return true;
	}


	static int Generate()
	{
		int result = 0;

		try
		{
			mParser = std::make_shared<ParsingDriver>();

			if (lexer_initialize(mParser, mInputFilename))
			{
#if 0
				// TODO:Unimplemented
				if (mVariableHeaderFile)
				{
					fprintf(mVariableHeaderFile, "#if !defined(___MANA_GLOBAL_H___)\n");
					fprintf(mVariableHeaderFile, "#define ___MANA_GLOBAL_H___\n");
					fprintf(mVariableHeaderFile, "typedef struct mana_global\n{\n");
				}
#endif
				result = mParser->Parse() != 0 || yynerrs != 0;
#if 0
				// TODO:Unimplemented
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
					Dump();
				}

				mParser->GetSymbolFactory()->CheckUndefine();

				if (result == 0)
				{
					OutputStream stream;
					if (GenerateBinary(stream))
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
					if (mOutputFilename[0] != '\0' && result != 0)
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

		lexer_finalize();

		return result;
	}

	static void PrintTitle()
	{
		MANA_PRINT("mana %s.%s (%04d-%02d-%02d)\n",
			mana::build::MajorVersion, mana::build::MinorVersion,
			mana::build::Year, mana::build::Month, mana::build::Day);
	}

	static void PrintCopyright()
	{
		MANA_PRINT("mana - Copyright (C) 2002-%04d Shun Moriya\n", mana::build::Year);
	}

	static void PrintUsage()
	{
		MANA_PRINT("usage:mana [switch] infile\n");
		MANA_PRINT("            -o filename     specify output file name\n");
		MANA_PRINT("            -i dirname      specify program header directory name\n");
		MANA_PRINT("            --help          print this message\n");
		MANA_PRINT("            --copyright     print copyright holder\n");
		MANA_PRINT("            --version       print the version\n");
		MANA_PRINT("\nReport bugs to https://github.com/shun126/Mana/issues\n");
	}

	static bool ParseArguments(int argc, char* argv[])
	{
		if (argc < 2)
		{
			MANA_PRINT("No input files\n");
			return false;
		}
		else
		{
			std::string globalTypeHeaderDirectoryName;
			bool outputBinary = false;

			mInputFilename[0] = '\0';
			mOutputFilename[0] = '\0';
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
						{
							MANA_PRINT("no output file name\n");
							return false;
						}
						else
						{
							strcpy(mOutputFilename, sizeof(mOutputFilename), argv[cmdcnt]);
							outputBinary = true;
						}
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
					}
					break;

					case '-':
						cmdptr++;
						if (strcmp(cmdptr, "copyright") == 0)
						{
							PrintCopyright();
							return false;
						}
						else if (strcmp(cmdptr, "version") == 0)
						{
							PrintTitle();
							return false;
						}
						else if (strcmp(cmdptr, "help") == 0)
						{
							PrintUsage();
							return false;
						}
						else if (strcmp(cmdptr, "debug") == 0)
						{
							mDebug = true;
							break;
						}
						else if (strcmp(cmdptr, "release") == 0)
						{
							mRelease = true;
							break;
						}

					default:
						MANA_PRINT("unrecognized option\n");
						return false;
					}
				}
				else if (mInputFilename[0] == '\0')
				{
					strcpy(mInputFilename, sizeof(mInputFilename), cmdptr);
				}
				else
				{
					MANA_PRINT("unrecognized option\n");
					return false;
				}
			}

			// check
			{
				char drive[_MAX_DRIVE];
				char dir[_MAX_DIR];
				char fname[_MAX_FNAME];
				char ext[_MAX_EXT];
				splitpath(mInputFilename, drive, sizeof(drive), dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));

				// output binary
				if (outputBinary)
				{
					makepath(mOutputFilename, sizeof(mOutputFilename), drive, dir, fname, ".mx");
				}

				// output global types
				if (!globalTypeHeaderDirectoryName.empty())
				{
					globalTypeHeaderDirectoryName += fname;
					globalTypeHeaderDirectoryName += ".h";

					mVariableHeaderFile.open(globalTypeHeaderDirectoryName, std::ios::out);
					if (!mVariableHeaderFile.is_open())
					{
						MANA_PRINT("'%s' open failed.\n", globalTypeHeaderDirectoryName.c_str());
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
#endif

	if (mana::ParseArguments(argc, argv))
	{
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
