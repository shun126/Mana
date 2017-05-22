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
	inline static std::shared_ptr<TypeDescriptor> GetVoidTypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Void);
	}

	inline static std::shared_ptr<TypeDescriptor> GetInt8TypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char);
	}

	inline static std::shared_ptr<TypeDescriptor> GetInt16TypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Short);
	}

	inline static std::shared_ptr<TypeDescriptor> GetInt32TypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int);
	}

	inline static std::shared_ptr<TypeDescriptor> GetFloat32TypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float);
	}

	inline static std::shared_ptr<TypeDescriptor> GetStringTypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->GetString();
	}

	inline static std::shared_ptr<TypeDescriptor> GetReferenceTypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Reference);
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
	TODO:string_viewの利用を検討して下さい
	*/
	static const std::string_view ToString(const char* text)
	{
		return GetSystemHolder().GetStringPool()->Set(text);
	}

	/*
	TODO:string_viewの利用を検討して下さい
	*/
	static const std::string_view LiteralToString(const char* text, const size_t length)
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

			return GetSystemHolder().GetStringPool()->Set(newBuffer);
		}
		else
		{
			CompileError("unterminated string literal");
			return std::string_view();
		}
	}
}
