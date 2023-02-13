/*!
mana (compiler)

@file	LexerPrivateFunction.h
@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "ParsingDriver.h"

namespace mana
{
	Lexer::Lexer(const std::shared_ptr<mana::ParsingDriver>& parsingDriver)
		: mParsingDriver(parsingDriver)
	{
	}

	Lexer::~Lexer()
	{
		if (!mContext.empty())
		{
			while (!Close())
				;
		}
	}

	bool Lexer::Open(const std::string_view filename, const bool check)
	{
		char path[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		if (mana::fullpath(path, filename.data(), _MAX_PATH) == NULL)
		{
			if (mCurrentPath.empty())
				mana::CompileError({ "unable to open \"", path, "\"" });
			else
				mana::Trace({ "unable to open \"", path, "\"\n"});
			return false;
		}

		const std::string_view pathPtr = mParsingDriver->GetStringPool()->Set(path);

		/*
		if(mana_debug)
			printf("lexer: %s\n", path);
			*/
			/*
			#if defined(_MSC_VER)
				char* path = path;
				while(*pathPointer)
				{
					*pathPointer = toupper(*pathPointer);
					++pathPointer;
				}
			#endif
			*/

		if ((check == true) && (mLexer->IsOpened(path) == false))
			return true;

#if defined(__STDC_WANT_SECURE_LIB__)
		if (fopen_s(&yyin, path, "rt") != 0)
#else
		if ((yyin = fopen(path, "rt")) == NULL)
#endif
		{
			const std::string message = Concat({ "unable to open \"", pathPtr, "\"\n" });
			if (mCurrentPath.empty())
			{
				mana::CompileError(message);
			}
			else
			{
				MANA_PRINT(message.c_str());
			}
			return false;
		}

		// save lineno
		if (!mContext.empty())
		{
			mContext.top()->mLineno = yylineno;
		}

		// switch!
		YY_BUFFER_STATE newBufferState = yy_create_buffer(yyin, YY_BUF_SIZE);
		yy_switch_to_buffer(newBufferState);
		mCurrentPath = pathPtr;
		yylineno = 1;

		std::unique_ptr<Context> nextContext = std::make_unique<Context>();
		nextContext->mBufferState = newBufferState;
		nextContext->mPath = pathPtr;
		nextContext->mLineno = yylineno;
		mContext.push(std::move(nextContext));

		// set currect directory
		mana::splitpath(path, drive, sizeof(drive), dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
		mana::makepath(path, sizeof(path), drive, dir, "", "");
		mana::chdir(path);

		return true;
	}

	bool Lexer::IsOpened(const std::string_view path)
	{
		if (mPathSet.find(path) == mPathSet.end())
		{
			mPathSet.insert(path);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Lexer::Close()
	{
		char path[_MAX_PATH];
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];

		yy_delete_buffer(YY_CURRENT_BUFFER);
		mContext.pop();

		if (mContext.empty())
		{
			return true;
		}
		else
		{
			yy_switch_to_buffer(mContext.top()->mBufferState);
			mCurrentPath = mContext.top()->mPath;
			yylineno = mContext.top()->mLineno;

			// set currect directory
			mana::splitpath(mCurrentPath.data(), drive, sizeof(drive), dir, sizeof(dir), fname, sizeof(fname), ext, sizeof(ext));
			mana::makepath(path, sizeof(path), drive, dir, "", "");
			mana::chdir(path);

			return false;
		}
	}

	const std::string& Lexer::GetCurrentFilename()
	{
		return mCurrentPath;
	}

	void Lexer::SetCurrentFilename(const std::string& filename)
	{
		mCurrentPath = filename;
	}

	mana::int_t Lexer::binary(const char* text)
	{
		text += 2;		// skip '0b'
		const char* sp = text;

		mana::int_t t0 = 0;
		mana::int_t t1 = 0;
		while (*text != '\0')
		{
			if (*text == '_')
			{
				++text;
				continue;
			}
			if (*text != '0' && *text != '1')
				break;
			++text;
			t0++;
		}
		while (t0 > 0)
		{
			if (*sp == '_')
			{
				sp++;
				continue;
			}
			t1 |= (*sp++ - '0') << (t0 - 1);
			t0--;
		}
		return t1;
	}
}
