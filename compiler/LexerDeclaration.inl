/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "Main.h"
#include "SystemHolder.h"
#include <set>
#include <stack>

	typedef struct Context
	{
		YY_BUFFER_STATE mBufferState;
		const char* mPath;
		mana::int_t mLineno;
	}Context;

	typedef struct Lexer
	{
		std::stack<std::unique_ptr<Context>> mContext;
		std::set<const char*> mPathSet;
		const char* mCurrentPath = nullptr;

		Lexer();
		~Lexer();

		bool Open(const char* filename, const bool check);
		bool IsOpened(const char* path);
		bool Close();

		const char* GetCurrentFilename(void);
		void SetCurrentFilename(const char* filename);

		static mana::int_t binary(const char* text);
	}Lexer;

namespace mana
{

	static const std::shared_ptr<TypeDescriptor>& GetVoidTypeDescriptor();
	static const std::shared_ptr<TypeDescriptor>& GetInt8TypeDescriptor();
	static const std::shared_ptr<TypeDescriptor>& GetInt16TypeDescriptor();
	static const std::shared_ptr<TypeDescriptor>& GetInt32TypeDescriptor();
	static const std::shared_ptr<TypeDescriptor>& GetFloat32TypeDescriptor();
	static const std::shared_ptr<TypeDescriptor>& GetStringTypeDescriptor();
	static const std::shared_ptr<TypeDescriptor>& GetReferenceTypeDescriptor();

	static int_t ToInt(const char* text);
	static int_t ToIntHex(const char* text);
	static float_t ToFloat(const char* text);
	static const char* ToString(const char* text);
	static const char* LiteralToString(const char* text, const size_t length);
}
