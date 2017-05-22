/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once

namespace mana
{
	static const std::shared_ptr<TypeDescriptor>& GetVoidTypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Void);
	}

	static const std::shared_ptr<TypeDescriptor>& GetInt8TypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Char);
	}

	static const std::shared_ptr<TypeDescriptor>& GetInt16TypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Short);
	}

	static const std::shared_ptr<TypeDescriptor>& GetInt32TypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Int);
	}

	static const std::shared_ptr<TypeDescriptor>& GetFloat32TypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Float);
	}

	static const std::shared_ptr<TypeDescriptor>& GetStringTypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->GetString();
	}

	static const std::shared_ptr<TypeDescriptor>& GetReferenceTypeDescriptor()
	{
		return GetSystemHolder().GetTypeDescriptorFactory()->Get(TypeDescriptor::Id::Reference);
	}

	static int_t ToInt(const char* text)
	{
		return static_cast<mana::int_t>(atoi(text));
	}

	static int_t ToIntHex(const char* text)
	{
		mana::int_t integer;
#if defined(MANA_TARGET_WINDOWS)
		sscanf_s(text, "%x", &integer);
#else
		sscanf(text, "%x", &integer);
#endif
		return integer;
	}

	static float_t ToFloat(const char* text)
	{
		return static_cast<float_t>(atof(text));
	}

	static const char* ToString(const char* text)
	{
		return GetSystemHolder().GetStringPool()->Set(text);
	}

	static const char* LiteralToString(const char* text, const size_t length)
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
					++yylineno;

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
			return "";
		}
	}
}
