/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
//#include "SystemHolder.h"
#include "StringPool.h"
#include <memory>
#include <set>
#include <stack>

struct Context
{
	YY_BUFFER_STATE mBufferState;
	std::string_view mPath;
	mana::int_t mLineNo;
};

namespace mana
{
	class ParsingDriver;

	class Lexer final
	{
	public:
		Lexer() = delete;
		Lexer(const Lexer& other) = delete;
		Lexer(Lexer&& other) noexcept = delete;
		Lexer operator=(const Lexer& other) = delete;
		Lexer operator=(Lexer&& other) noexcept = delete;
		explicit Lexer(const std::shared_ptr<ParsingDriver>& parsingDriver);
		~Lexer();

		bool Open(const std::string_view& filename, const bool check);
		bool IsOpened(const std::string_view& path);
		bool Close();

		const std::string& GetCurrentFilename();
		void SetCurrentFilename(const std::string& filename);

		static int_t Binary(const char* text);

	public:
		std::shared_ptr<ParsingDriver> mParsingDriver;
		std::stack<std::unique_ptr<Context>> mContext;
		std::set<std::string_view> mPathSet;
		std::string mCurrentPath;
	};
}

namespace mana
{
	namespace lexer
	{
		static std::shared_ptr<TypeDescriptor> GetVoidTypeDescriptor();
		static std::shared_ptr<TypeDescriptor> GetInt8TypeDescriptor();
		static std::shared_ptr<TypeDescriptor> GetInt16TypeDescriptor();
		static std::shared_ptr<TypeDescriptor> GetInt32TypeDescriptor();
		static std::shared_ptr<TypeDescriptor> GetFloat32TypeDescriptor();
		static std::shared_ptr<TypeDescriptor> GetStringTypeDescriptor();
		static std::shared_ptr<TypeDescriptor> GetReferenceTypeDescriptor();

		static int_t ToInt(const std::string_view text);
		static int_t ToIntHex(const std::string_view text);
		static float_t ToFloat(const std::string_view text);
		static std::string_view ToString(const char* text);
		static std::string_view LiteralToString(const char* text, const size_t length);
	}

	static std::shared_ptr<Lexer> LexerInstance;
}
