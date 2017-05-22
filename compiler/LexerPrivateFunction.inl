/*!
mana (compiler)

@file	LexerPrivateFunction.h
@author	Shun Moriya
@date	2017-
*/

#pragma once

Lexer::Lexer()
{
	/*
	pool_initialize();
	parser_initialize();
	*/
}

Lexer::~Lexer()
{
	if (!mContext.empty())
	{
		while (!Close())
			;
	}
	//parser_finalize();
	//pool_finalize();
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
		{
			mana::CompileError("unable to open \"%s\"", path);
		}
		else
		{
			MANA_PRINT("unable to open \"%s\"\n", path);
		}
		return false;
	}

	const std::string_view pathPtr = mana::GetSystemHolder().GetStringPool()->Set(path);

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

	if((check == true) && (mLexer->IsOpened(path) == false))
		return true;

#if defined(__STDC_WANT_SECURE_LIB__)
	if (fopen_s(&yyin, path, "rt") != 0)
#else
	if ((yyin = fopen(path, "rt")) == NULL)
#endif
	{
		if (mCurrentPath.empty())
		{
			mana::CompileError("unable to open \"%s\"", pathPtr.data());
		}
		else
		{
			MANA_PRINT("unable to open \"%s\"\n", pathPtr.data());
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
	if(mPathSet.find(path) == mPathSet.end())
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

const std::string_view Lexer::GetCurrentFilename(void)
{
	return mCurrentPath;
}

void Lexer::SetCurrentFilename(const std::string_view filename)
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
