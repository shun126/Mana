/*!
mana (compiler)

@file	type.c
@brief	型情報に関するソースファイル
@detail	このファイルは型情報に関係するソースファイルです。
@author	Shun Moriya
@date	2003-
*/

#if !defined(___MANA_MAIN_H___)
#include "main.h"
#endif
#if !defined(___MANA_ERROR_H___)
#include "error.h"
#endif
#if !defined(___MANA_SYMBOL_H___)
#include "symbol.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "type.h"
#endif
#include <assert.h>

static char* mana_base_type_name[] = {
	"void",
	"char",
	"short",
	"int",
	"float",
	"pointer"
};

static int32_t mana_base_type_size[] = {
	IBSZ,												/* void */
	CBSZ,												/* int8_t */
	SBSZ,												/* int16_t */
	IBSZ,												/* int32_t */
	FBSZ,												/* float */
	IBSZ												/* pointer */
};

#define NUMBER_OF_BASE_TYPE sizeof(mana_base_type_name)/sizeof(mana_base_type_name[0])

type_description* mana_type_actor;
type_description* mana_type_nil;
type_description* mana_type_string;
type_description* mana_type_pointer;

static type_description* mana_type_basic_type_description[SYMBOL_DATA_TYPE_NUMBER_OF];
static type_description* mana_type_root_pointer = NULL;

void mana_type_initialize(void)
{
	for(size_t i = SYMBOL_DATA_TYPE_VOID; i < NUMBER_OF_BASE_TYPE; i++)
	{
		mana_type_basic_type_description[i] = mana_type_create((symbol_data_type_id)i, NULL, NULL);
		mana_type_basic_type_description[i]->name = mana_base_type_name[i];
		mana_type_basic_type_description[i]->memory_size = mana_base_type_size[i];
		mana_type_basic_type_description[i]->alignment_memory_size = mana_base_type_size[i];
	}

	/* self/sender解決用のダミーactor '参照' 型 */
	mana_type_actor = mana_type_create(SYMBOL_DATA_TYPE_ACTOR, NULL, NULL);
	mana_type_actor->alignment_memory_size = sizeof(void*);
	mana_type_actor->memory_size = sizeof(void*);
	mana_type_actor->name = "actor";

	/* nil解決用のダミーnil型 */
	mana_type_nil = mana_type_create(SYMBOL_DATA_TYPE_NIL, NULL, NULL);
	mana_type_nil->alignment_memory_size = CBSZ;
	mana_type_nil->memory_size = CBSZ;
	mana_type_nil->name = "nil";

	/* string型 (const char*) */
	mana_type_string = mana_type_create_reference(mana_type_basic_type_description[SYMBOL_DATA_TYPE_CHAR]);
	mana_type_string->name = "string";

	/* pointer型(const void*)  */
	mana_type_pointer = mana_type_create_reference(mana_type_basic_type_description[SYMBOL_DATA_TYPE_VOID]);
	mana_type_pointer->name = "pointer";
}

void mana_type_finalize(void)
{
	type_description* type = mana_type_root_pointer;
	while (type)
	{
		type_description* next = type->link;
		mana_free(type);
		type = next;
	}
	mana_type_root_pointer = NULL;
}

type_description* mana_type_get(symbol_data_type_id type)
{
	switch(type)
	{
	case SYMBOL_DATA_TYPE_ACTOR:
		return mana_type_actor;

	case SYMBOL_DATA_TYPE_NIL:
		return mana_type_nil;

	default:
		return mana_type_basic_type_description[type];
	}
}

type_description* mana_type_create(symbol_data_type_id tcons, type_description* component, type_description* parent)
{
	type_description* type;
	type = (type_description*)mana_calloc(1, sizeof(type_description));
#if defined(_DEBUG)
	static uint32_t count = 0;
	snprintf(type->magic, sizeof(type->magic), "T%d", count);
	++count;
#endif
	type->tcons = tcons;
	type->parent = parent;
	type->component = component;
	type->number_of_elements = 1;

	type->link = mana_type_root_pointer;
	mana_type_root_pointer = type;

	return type;
}

type_description* mana_type_create_reference(type_description* self)
{
	type_description* type;

	type = mana_type_create(SYMBOL_DATA_TYPE_REFERENCE, self, NULL);
	type->memory_size = type->alignment_memory_size = mana_base_type_size[SYMBOL_DATA_TYPE_REFERENCE];
	return type;
}

type_description* mana_type_create_array(int32_t number_of_elements)
{
	type_description* type;
	type = mana_type_create(SYMBOL_DATA_TYPE_ARRAY, NULL, NULL);
	type->number_of_elements = number_of_elements;
	return type;
}

void mana_type_set_array(type_description* self, type_description* ElmnT)
{
	if(self->component == NULL)
	{
		if(self != ElmnT)
			self->component = ElmnT;
	}
	else
	{
		assert(self->component != self);
		mana_type_set_array(self->component, ElmnT);
		ElmnT = self->component;
	}
	self->alignment_memory_size = ElmnT->alignment_memory_size;
	self->memory_size = self->number_of_elements < 0 ? 0 :	ElmnT->memory_size * self->number_of_elements;
}

int32_t mana_type_compare(type_description* self, type_description* T2)
{
	if(self->tcons != T2->tcons)
		return false;

	switch(self->tcons)
	{
	case SYMBOL_DATA_TYPE_REFERENCE:
		return mana_type_compare(self->component, T2->component);

	case SYMBOL_DATA_TYPE_ARRAY:
		{
			int32_t n1 = self->number_of_elements;
			int32_t n2 = T2->number_of_elements;
			if(n1 > 0 && n2 > 0 && n1 != n2 )
				mana_compile_error("array size unmatched");
			return mana_type_compare(self->component, T2->component);
		}

	case SYMBOL_DATA_TYPE_ACTOR:
		return true;

	case SYMBOL_DATA_TYPE_STRUCT:
	case SYMBOL_DATA_TYPE_INCOMPLETE:
	default:
		return (self == T2);
	}
}

int32_t mana_type_compatible(type_description* self, type_description* ActT)
{
	switch(self->tcons)
	{
#if 0
	case SYMBOL_DATA_TYPE_CHAR:
		if(ActT->tcons == SYMBOL_DATA_TYPE_CHAR)
			return true;
		break;

	case SYMBOL_DATA_TYPE_SHORT:
		switch(ActT->tcons)
		{
		case SYMBOL_DATA_TYPE_CHAR:
		case SYMBOL_DATA_TYPE_SHORT:
			return true;
		default:
			break;
		}
		break;
#else
	case SYMBOL_DATA_TYPE_CHAR:
	case SYMBOL_DATA_TYPE_SHORT:
#endif
	case SYMBOL_DATA_TYPE_INT:
		switch(ActT->tcons)
		{
		case SYMBOL_DATA_TYPE_CHAR:
		case SYMBOL_DATA_TYPE_SHORT:
		case SYMBOL_DATA_TYPE_INT:
			return true;
		default:
			break;
		}
		break;

	case SYMBOL_DATA_TYPE_FLOAT:
		if(ActT->tcons == SYMBOL_DATA_TYPE_FLOAT)
			return true;
		break;

	case SYMBOL_DATA_TYPE_STRUCT:
	case SYMBOL_DATA_TYPE_ARRAY:
		if(mana_type_compare(self, ActT))
			return true;
		break;

	case SYMBOL_DATA_TYPE_ACTOR:
		if(mana_type_compare(self, ActT))
			return true;

		if(ActT->tcons == SYMBOL_DATA_TYPE_NIL)
			return true;
		break;

	case SYMBOL_DATA_TYPE_REFERENCE:
		if(self == ActT)
			return true;
		break;

	default:
		break;
	}

	mana_compile_error("incompatible type of operand");

	return false;
}

void mana_type_print(FILE* fp, type_description* type)
{
	static char* type_name[] = {
		"void",
		"byte",
		"char",
		"short",
		"int",
		"float",
		"ref(",
		"array[",
		"struct{",
		"actor{",
		"incomplete"
	};
	if(type == NULL)
		return;

	fprintf(fp, "%s", type_name[type->tcons]);
	switch(type->tcons)
	{
	case SYMBOL_DATA_TYPE_REFERENCE:
		mana_type_print(fp, type->component); printf(")");
		break;

	case SYMBOL_DATA_TYPE_ARRAY:
		mana_type_print(fp, type->component);
		if(type->number_of_elements >= 0)
			fprintf(fp, ", %d", type->number_of_elements);
		else
			fprintf(fp, ", *");
		fprintf(fp, "]");
		break;

	case SYMBOL_DATA_TYPE_STRUCT:
		fprintf(fp, "%d bytes(%d)}", type->memory_size, type->alignment_memory_size);
		break;

	case SYMBOL_DATA_TYPE_ACTOR:
		fprintf(fp, "%d bytes(%d)}", type->memory_size, type->alignment_memory_size);
		break;

	default:
		break;
	}
}
