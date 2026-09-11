/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "ParsingDriver.h"

namespace mana
{
	inline Lexer::Lexer(const std::shared_ptr<mana::ParsingDriver>& parsingDriver, const std::shared_ptr<SourceResolver>& sourceResolver)
		: mParsingDriver(parsingDriver)
		, mSourceResolver(sourceResolver)
	{
	}

	inline Lexer::~Lexer()
	{
		if (!mContext.empty())
		{
			while (!Close())
				;
		}
	}

	/*!
	改行コードを LF へ揃えます

	字句解析器の規則は CR を扱わないため、読み込んだ時点で取り除きます。
	*/
	inline void Lexer::NormalizeNewlines(std::string& text)
	{
		size_t write = 0;
		for (size_t read = 0; read < text.size(); ++read)
		{
			if (text[read] == '\r')
			{
				// CRLF の CR は捨て、単独の CR は LF に読み替えます
				if (read + 1 < text.size() && text[read + 1] == '\n')
					continue;
				text[write++] = '\n';
			}
			else
			{
				text[write++] = text[read];
			}
		}
		text.resize(write);
	}

	inline bool Lexer::Open(const std::string_view& filename, const bool check)
	{
		const std::string path = mSourceResolver->Resolve(mCurrentPath, filename);
		if (path.empty())
		{
			mana::CompileError({ "unable to open \"", filename, "\"" });
			return false;
		}

		if (check == true && IsOpened(path) == false)
		{
			// import は同じソースを二度読み込みません
			return true;
		}

		std::string text;
		if (!mSourceResolver->Read(path, text))
		{
			mana::CompileError({ "unable to open \"", path, "\"" });
			return false;
		}
		NormalizeNewlines(text);

		// save lineno
		if (!mContext.empty())
		{
			mContext.top()->mLineNo = yylineno;
		}

		// switch! yy_scan_bytes は内容を複製して現在のバッファを切り替えます
		YY_BUFFER_STATE newBufferState = yy_scan_bytes(text.data(), static_cast<int>(text.size()));
		if (newBufferState == nullptr)
		{
			mana::FatalNoMemory();
			return false;
		}
		mCurrentPath = path;
		yylineno = 1;

		std::unique_ptr<Context> nextContext = std::make_unique<Context>();
		nextContext->mBufferState = newBufferState;
		nextContext->mPath = path;
		nextContext->mLineNo = yylineno;
		mContext.push(std::move(nextContext));

		return true;
	}

	inline bool Lexer::IsOpened(const std::string& path)
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

	inline bool Lexer::Close()
	{
		yy_delete_buffer(YY_CURRENT_BUFFER);
		mContext.pop();

		if (mContext.empty())
		{
			mCurrentPath.clear();
			return true;
		}
		else
		{
			yy_switch_to_buffer(mContext.top()->mBufferState);
			mCurrentPath = mContext.top()->mPath;
			yylineno = static_cast<int>(mContext.top()->mLineNo);

			return false;
		}
	}

	inline const std::string& Lexer::GetCurrentFilename()
	{
		return mCurrentPath;
	}

	inline void Lexer::SetCurrentFilename(const std::string& filename)
	{
		mCurrentPath = filename;
	}

	inline int_t Lexer::Binary(const char* text)
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
