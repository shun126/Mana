/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once

#if defined(MANA_TARGET_UNIX)
#include <alloca.h>
#endif

namespace mana
{
	namespace lexer
	{
		bool Initialize(const std::shared_ptr<mana::ParsingDriver>& parsingDriver, const std::string_view& filename)
		{
			if (LexerInstance == nullptr)
			{
				LexerInstance = std::make_shared<Lexer>(parsingDriver);
				yylineno = 1;
			}
			return LexerInstance->Open(filename, true);
		}

		void Finalize()
		{
			if (LexerInstance)
			{
				LexerInstance.reset();
				yylex_destroy();
			}
		}

		bool Open(const std::string_view& filename, const bool check)
		{
			return LexerInstance->Open(filename, check);
		}

		bool Close()
		{
			return LexerInstance->Close();
		}

		const std::string& GetCurrentFilename()
		{
			return LexerInstance->GetCurrentFilename();
		}

		void SetCurrentFilename(const std::string& filename)
		{
			LexerInstance->SetCurrentFilename(filename);
		}

		int GetCurrentLineNo()
		{
			return yylineno;
		}

		void SetCurrentLineNo(const int lineNo)
		{
			yylineno = lineNo;
		}

		inline static std::shared_ptr<TypeDescriptor> GetVoidTypeDescriptor()
		{
			return LexerInstance->mParsingDriver->GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Void);
		}

		inline static std::shared_ptr<TypeDescriptor> GetInt8TypeDescriptor()
		{
			return LexerInstance->mParsingDriver->GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char);
		}

		inline static std::shared_ptr<TypeDescriptor> GetInt16TypeDescriptor()
		{
			return LexerInstance->mParsingDriver->GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Short);
		}

		inline static std::shared_ptr<TypeDescriptor> GetInt32TypeDescriptor()
		{
			return LexerInstance->mParsingDriver->GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int);
		}

		inline static std::shared_ptr<TypeDescriptor> GetBoolTypeDescriptor()
		{
			return LexerInstance->mParsingDriver->GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Bool);
		}

		inline static std::shared_ptr<TypeDescriptor> GetFloat32TypeDescriptor()
		{
			return LexerInstance->mParsingDriver->GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float);
		}

		inline static std::shared_ptr<TypeDescriptor> GetStringTypeDescriptor()
		{
			return LexerInstance->mParsingDriver->GetTypeDescriptorFactory()->GetString();
		}

		inline static std::shared_ptr<TypeDescriptor> GetReferenceTypeDescriptor()
		{
			return LexerInstance->mParsingDriver->GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Reference);
		}

		inline static int_t ToInt(const std::string_view text)
		{
			return static_cast<mana::int_t>(atoi(text.data()));
		}

		static int_t ToIntHex(const std::string_view text)
		{
			mana::int_t integer;
#if defined(MANA_TARGET_WINDOWS)
			sscanf_s(text.data(), "%x", &integer);
#else
			sscanf(text.data(), "%x", &integer);
#endif
			return integer;
		}

		inline static float_t ToFloat(const std::string_view text)
		{
			return static_cast<float_t>(atof(text.data()));
		}

		/*
		*/
		static std::string_view ToString(const char* text)
		{
			return LexerInstance->mParsingDriver->GetStringPool()->Set(text);
		}

		/*
		*/
		static std::string_view LiteralToString(const char* text, const size_t length)
		{
			if ((text[0] == '\"') && (text[length - 1] == '\"'))
			{
				char* newBuffer = static_cast<char*>(alloca(length - 1));
				char* q = newBuffer;

				for (const char* p = &text[1]; p < text + length - 1; ++p)
				{
					switch (*p)
					{
					case '\\':
						switch (*(++p))
						{
						case 'n':	*q = '\n';	break;
						case 't':	*q = '\t';	break;
						case '\\':	*q = '\\';	break;
						}
						break;

					case '\n':
						//// ++yylineno;

					default:
						*q = *p;
						break;
					}
					++q;
				}
				*q = '\0';

				return LexerInstance->mParsingDriver->GetStringPool()->Set(newBuffer);
			}
			else
			{
				CompileError("unterminated string literal");
				return "";
			}
		}
	}
}
