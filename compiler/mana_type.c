/*
 * mana (compiler)
 *
 * @file	mana_symbol.c
 * @brief	型情報に関するソースファイル
 * @detail	このファイルは型情報に関係するソースファイルです。
 * @author	Shun Moriya <shun@mnu.sakura.ne.jp>
 * @date	2003-
 */

#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif
#if !defined(___MANA_SYMBOL_H___)
#include "mana_symbol.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "mana_type.h"
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

static int mana_base_type_size[] = {
	IBSZ,												/* void */
	CBSZ,												/* char */
	SBSZ,												/* short */
	IBSZ,												/* int */
	FBSZ,												/* float */
	IBSZ												/* pointer */
};

#define NUMBER_OF_BASE_TYPE sizeof(mana_base_type_name)/sizeof(mana_base_type_name[0])

mana_type_description* mana_type_actor;
mana_type_description* mana_type_nil;
mana_type_description* mana_type_string;
mana_type_description* mana_type_pointer;

static mana_type_description* mana_type_basic_type_description[MANA_DATA_TYPE_NUMBER_OF];

void mana_type_initialize(void)
{
	int i;

	for(i = (int)MANA_DATA_TYPE_VOID; i < (int)NUMBER_OF_BASE_TYPE; i++)
	{
		mana_type_basic_type_description[i] = mana_type_create((mana_symbol_data_type_id)i, NULL, NULL);
		mana_type_basic_type_description[i]->name = mana_base_type_name[i];
		mana_type_basic_type_description[i]->memory_size = mana_base_type_size[i];
		mana_type_basic_type_description[i]->alignment_memory_size = mana_base_type_size[i];
	}

	/* self/sender解決用のダミーactor '参照' 型 */
	mana_type_actor = mana_type_create(MANA_DATA_TYPE_ACTOR, NULL, NULL);
	mana_type_actor->alignment_memory_size = sizeof(void*);
	mana_type_actor->memory_size = sizeof(void*);
	mana_type_actor->name = "actor";

	/* nil解決用のダミーnil型 */
	mana_type_nil = mana_type_create(MANA_DATA_TYPE_NIL, NULL, NULL);
	mana_type_nil->alignment_memory_size = CBSZ;
	mana_type_nil->memory_size = CBSZ;
	mana_type_nil->name = "nil";

	/* string型 (const char*) */
	mana_type_string = mana_type_create_reference(mana_type_basic_type_description[MANA_DATA_TYPE_CHAR]);
	mana_type_string->name = "string";

	/* pointer型(const void*)  */
	mana_type_pointer = mana_type_create_reference(mana_type_basic_type_description[MANA_DATA_TYPE_VOID]);
	mana_type_pointer->name = "pointer";
}

void mana_type_finalize(void)
{
	int i;

	for(i = (int)MANA_DATA_TYPE_VOID; i < (int)NUMBER_OF_BASE_TYPE; i++)
	{
		mana_free(mana_type_basic_type_description[i]);
	}
	mana_free(mana_type_actor);
	mana_free(mana_type_nil);
	mana_free(mana_type_string);
	mana_free(mana_type_pointer);
}

mana_type_description* mana_type_get(mana_symbol_data_type_id type)
{
	switch(type)
	{
	case MANA_DATA_TYPE_ACTOR:
		return mana_type_actor;

	case MANA_DATA_TYPE_NIL:
		return mana_type_nil;

	default:
		return mana_type_basic_type_description[type];
	}
}

mana_type_description* mana_type_create(mana_symbol_data_type_id tcons, mana_type_description* component, mana_type_description* parent)
{
	mana_type_description* type;
	type = (mana_type_description*)mana_calloc(1, sizeof(mana_type_description));
	type->tcons = tcons;
	type->parent = parent;
	type->component = component;
	type->number_of_elements = 1;
	return type;
}

mana_type_description* mana_type_create_reference(mana_type_description* self)
{
	mana_type_description* type;

	type = mana_type_create(MANA_DATA_TYPE_REFERENCE, self, NULL);
	type->memory_size = type->alignment_memory_size = mana_base_type_size[MANA_DATA_TYPE_REFERENCE];
	return type;
}

mana_type_description* mana_type_create_array(int number_of_elements)
{
	mana_type_description* type;
	type = mana_type_create(MANA_DATA_TYPE_ARRAY, NULL, NULL);
	type->number_of_elements = number_of_elements;
	return type;
}

void mana_type_set_array(mana_type_description* self, mana_type_description* ElmnT)
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

int mana_type_compare(mana_type_description* self, mana_type_description* T2)
{
	if(self->tcons != T2->tcons)
		return MANA_FALSE;

	switch(self->tcons)
	{
	case MANA_DATA_TYPE_REFERENCE:
		return mana_type_compare(self->component, T2->component);

	case MANA_DATA_TYPE_ARRAY:
		{
			int n1 = self->number_of_elements;
			int n2 = T2->number_of_elements;
			if(n1 > 0 && n2 > 0 && n1 != n2 )
				mana_compile_error("array size unmatched");
			return mana_type_compare(self->component, T2->component);
		}

	case MANA_DATA_TYPE_ACTOR:
		return MANA_TRUE;

	case MANA_DATA_TYPE_STRUCT:
	case MANA_DATA_TYPE_INCOMPLETE:
	default:
		return (self == T2);
	}
}

int mana_type_compatible(mana_type_description* self, mana_type_description* ActT)
{
	switch(self->tcons)
	{
#if 0
	case MANA_DATA_TYPE_CHAR:
		if(ActT->tcons == MANA_DATA_TYPE_CHAR)
			return MANA_TRUE;
		break;

	case MANA_DATA_TYPE_SHORT:
		switch(ActT->tcons)
		{
		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
			return MANA_TRUE;
		default:
			break;
		}
		break;
#else
	case MANA_DATA_TYPE_CHAR:
	case MANA_DATA_TYPE_SHORT:
#endif
	case MANA_DATA_TYPE_INT:
		switch(ActT->tcons)
		{
		case MANA_DATA_TYPE_CHAR:
		case MANA_DATA_TYPE_SHORT:
		case MANA_DATA_TYPE_INT:
			return MANA_TRUE;
		default:
			break;
		}
		break;

	case MANA_DATA_TYPE_FLOAT:
		if(ActT->tcons == MANA_DATA_TYPE_FLOAT)
			return MANA_TRUE;
		break;

	case MANA_DATA_TYPE_STRUCT:
	case MANA_DATA_TYPE_ARRAY:
		if(mana_type_compare(self, ActT))
			return MANA_TRUE;
		break;

	case MANA_DATA_TYPE_ACTOR:
		if(mana_type_compare(self, ActT))
			return MANA_TRUE;

		if(ActT->tcons == MANA_DATA_TYPE_NIL)
			return MANA_TRUE;
		break;

	case MANA_DATA_TYPE_REFERENCE:
		if(self == ActT)
			return MANA_TRUE;
		break;

	default:
		break;
	}

	mana_compile_error("incompatible type of operand");

	return MANA_FALSE;
}

void mana_type_print(FILE* fp, mana_type_description* type)
{
	static char *type_name[] = {
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
	case MANA_DATA_TYPE_REFERENCE:
		mana_type_print(fp, type->component); printf(")");
		break;

	case MANA_DATA_TYPE_ARRAY:
		mana_type_print(fp, type->component);
		if(type->number_of_elements >= 0)
			fprintf(fp, ", %d", type->number_of_elements);
		else
			fprintf(fp, ", *");
		fprintf(fp, "]");
		break;

	case MANA_DATA_TYPE_STRUCT:
		fprintf(fp, "%d bytes(%d)}", type->memory_size, type->alignment_memory_size);
		break;

	case MANA_DATA_TYPE_ACTOR:
		fprintf(fp, "%d bytes(%d)}", type->memory_size, type->alignment_memory_size);
		break;

	default:
		break;
	}
}
