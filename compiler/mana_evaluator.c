/*
mana (compiler)

@file	mana_evaluator.c
@brief	\•¶–Ø•]‰¿‚ÉŠÖ‚·‚éƒ\[ƒXƒtƒ@ƒCƒ‹
@detail	‚±‚Ìƒtƒ@ƒCƒ‹‚Í\•¶–Ø•]‰¿‚ÉŠÖŒW‚·‚éƒ\[ƒXƒtƒ@ƒCƒ‹‚Å‚·B
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_EVALUATOR_H___)
#include "mana_evaluator.h"
#endif

#if !defined(___MANA_TYPE_H___)
#include "mana_type.h"
#endif
#if !defined(___MANA_MAIN_H___)
#include "mana_main.h"
#endif

static struct mana_evaluator_object
{
	bool static_block_opend;
}mana_evaluator_object;

void mana_evaluator_initialize(void)
{
	{
		/* vec2 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("vec2");

		/* vec3 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("z", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("vec3");

		/* vec4 */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("z", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("w", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("vec4");

		/* color */
		mana_symbol_open_structure();
		mana_symbol_allocate_memory(mana_symbol_create_identification("r", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("g", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("b", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_allocate_memory(mana_symbol_create_identification("a", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_NORMAL);
		mana_symbol_close_structure("color");
	}

	{
		mana_symbol_entry* symbol;

		/* int getUerData() */
		symbol = mana_symbol_create_function("getUserData");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* void setUserData(int data) */
		symbol = mana_symbol_create_function("setUserData");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("data", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* pointer getUserPointer() */
		symbol = mana_symbol_create_function("getUserPointer");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_pointer);

		/* void setUserPointer(pointer address) */
		symbol = mana_symbol_create_function("setUserPointer");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("address", NULL, false), mana_type_pointer, MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* int debug() */
		symbol = mana_symbol_create_function("debug");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* void setTickCount(int count) */
		symbol = mana_symbol_create_function("setTickCount");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("count", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void wait(float) */
		symbol = mana_symbol_create_function("wait");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("second", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void waitFrame(int) */
		symbol = mana_symbol_create_function("waitFrame");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("frame", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* void srand(int count) */
		symbol = mana_symbol_create_function("srand");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("count", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_VOID));

		/* int rand(int, int) */
		symbol = mana_symbol_create_function("rand");
		symbol->number_of_parameters = 2;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("min", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_allocate_memory(mana_symbol_create_identification("max", NULL, false), mana_type_get(MANA_DATA_TYPE_INT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_INT));

		/* float frand() */
		symbol = mana_symbol_create_function("frand");
		symbol->number_of_parameters = 0;
		mana_symbol_begin_native_function_registration();
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float sin(float) */
		symbol = mana_symbol_create_function("sin");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float cos(float) */
		symbol = mana_symbol_create_function("cos");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float atan2(float, float) */
		symbol = mana_symbol_create_function("atan2");
		symbol->number_of_parameters = 2;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("y", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_allocate_memory(mana_symbol_create_identification("x", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float tan(float) */
		symbol = mana_symbol_create_function("tan");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float angleMod(float) */
		symbol = mana_symbol_create_function("angleMod");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* float sqrt(float) */
		symbol = mana_symbol_create_function("sqrt");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("degree", NULL, false), mana_type_get(MANA_DATA_TYPE_FLOAT), MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_FLOAT));

		/* actor getActor(string) */
		symbol = mana_symbol_create_function("getActor");
		symbol->number_of_parameters = 1;
		mana_symbol_begin_native_function_registration();
		mana_symbol_allocate_memory(mana_symbol_create_identification("name", NULL, false), mana_type_string, MANA_MEMORY_TYPE_PARAMETER);
		mana_symbol_commit_native_function_registration(symbol, mana_type_get(MANA_DATA_TYPE_ACTOR));
	}
}

void mana_evaluator_finalize(void)
{
}

/*!
mana_symbol_lookup‚ðŒÄ‚Ño‚µmana_symbol‚ðŒŸõ‚µ‚Änode‚ÉÝ’è‚µ‚Ü‚·
@param	node	MANA_NODE_IDENTIFIERƒm[ƒh
*/
static void mana_compiler_resolve_identifier(mana_node* node)
{
	MANA_ASSERT(node);

	if (node->symbol == NULL)
	{
		node->symbol = mana_symbol_lookup(node->string);
		if (node->symbol)
		{
			if (node->type == NULL)
				node->type = node->symbol->type;
		}
		else
		{
			mana_compile_error("incomplete type name '%s'", node->string);
			node->type = mana_type_get(MANA_DATA_TYPE_INT);
		}
	}
}

/*!
node‚É“o˜^‚³‚ê‚½type‚ª–³‚¯‚ê‚ÎƒVƒ“ƒ{ƒ‹‚ðŒŸõ‚µ‚Ä‚»‚Ìtype‚ðnode‚ÉÝ’è‚µ‚Ü‚·
@param[in]	node	MANA_NODE_TYPE_DESCRIPTIONƒm[ƒh
*/
static void mana_compiler_resolve_type_description(mana_node* node)
{
	MANA_ASSERT(node);

	if (node->type == NULL)
	{
		node->symbol = mana_symbol_lookup(node->string);
		if (node->symbol)
		{
			node->type = node->symbol->type;
		}
		else
		{
			mana_compile_error("incomplete type name '%s'", node->string);
			node->type = mana_type_get(MANA_DATA_TYPE_INT);
		}
	}
}

/*!
mana_type_create_array‚ðŽg‚Á‚Ä”z—ñ‚ÌŒ^‚ðnew‚µ‚Ü‚·
@param[in]	node	MANA_NODE_VARIABLE_SIZEƒm[ƒh
@return		”z—ñmana_type_description
*/
static mana_type_description* mana_compiler_resolve_variable_size(mana_node* node)
{
	if (node == NULL)
		return NULL;

	MANA_ASSERT(node->left == NULL);
	MANA_ASSERT(node->right == NULL);

	if (node->string)
	{
		mana_symbol_entry* symbol = mana_symbol_lookup(node->string);
		if (symbol)
		{
			if (symbol->class_type == MANA_CLASS_TYPE_CONSTANT_INT)
			{
				node->type = mana_type_create_array(symbol->address);
			}
			else
			{
				mana_compile_error("invalid size information on parameter");
			}
		}
		else
		{
			mana_compile_error("identifier %s is not defined", node->string);
		}
	}
	else
	{
		if (node->digit > 0)
			node->type = mana_type_create_array(node->digit);
		else
			mana_compile_error("invalid size information on parameter");
	}

	if (node->type)
	{
		node->type->component = mana_compiler_resolve_variable_size(node->left);
	}

	return node->type;
}

/*!
mana_symbol_create_identification‚ðŒÄ‚Ño‚µ
mana_symbol‚ðnew‚µ‚Änode‚ÉÝ’è‚µ‚Ü‚·
@param[in]	node	MANA_NODE_DECLARATORƒm[ƒh
*/
static void mana_compiler_resolve_declarator(mana_node* node)
{
	MANA_ASSERT(node);

	if (node->symbol != NULL)
		return;

	mana_type_description* type = NULL;
	if (node->left && node->left->id == MANA_NODE_VARIABLE_SIZE)
		type = mana_compiler_resolve_variable_size(node->left);

	node->symbol = mana_symbol_create_identification(node->string, type, mana_evaluator_object.static_block_opend);
}

/*!
—¼•Ó‚ÌMANA_NODE_TYPE_DESCRIPTION‚ÆMANA_NODE_DECLARATOR‚ð‰ðŒˆ‚µ‚Ä
mana_symbol_allocate_memory‚ðŽg‚Á‚Äƒƒ‚ƒŠ‚ðŠ„‚è“–‚Ä‚Ü‚·
@param[in]	node			MANA_NODE_DECLARE_VARIABLEƒm[ƒh
@param[in]	memory_type_id	mana_symbol_memory_type_id
*/
static void mana_compiler_resolve_variable_description(mana_node* node, const mana_symbol_memory_type_id memory_type_id)
{
	MANA_ASSERT(node);
	
	MANA_ASSERT(node->left && node->left->id == MANA_NODE_TYPE_DESCRIPTION);
	mana_compiler_resolve_type_description(node->left);
	
	MANA_ASSERT(node->right && node->right->id == MANA_NODE_DECLARATOR);
	mana_compiler_resolve_declarator(node->right);

	mana_symbol_allocate_memory(node->right->symbol, node->left->type, memory_type_id);
}

/*!
ˆø”‚Ì”‚ðŽæ“¾‚µ‚Ü‚·iÄ‹AŒÄ‚Ño‚µj
@param	count	ˆø”‚Ì”Ô†
@param	param	ˆø”‚Ìmana_symbol_entry
@param	node	ˆø”‚Ìmana_node
@return	ˆø”‚Ì”
*/
static int32_t mana_evaluator_get_argument_count(const int32_t count, const mana_node* node)
{
	int32_t _count = count;
	if (node)
	{
		MANA_ASSERT(node->id == MANA_NODE_DECLARE_ARGUMENT);
		_count = mana_evaluator_get_argument_count(_count, node->right) + 1;
	}
	return _count;
}

void mana_evaluator_inherit_type_from_child_node(mana_node* node)
{
	MANA_ASSERT(node);

	// Ž©•ª‚ÌŒ^‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
	if (node->type == NULL)
	{
		// ƒVƒ“ƒ{ƒ‹‚ª“o˜^‚³‚ê‚Ä‚¢‚é‚È‚çAƒVƒ“ƒ{ƒ‹‚ÌŒ^‚ðŒp³‚·‚é
		if (node->symbol && node->symbol->type)
		{
			node->type = node->symbol->type;
		}
		else if (node->left)
		{
			// ¶•Ó‚ÌƒVƒ“ƒ{ƒ‹‚ª“o˜^‚³‚ê‚Ä‚¢‚é‚È‚çA¶•Ó‚ÌƒVƒ“ƒ{ƒ‹‚ÌŒ^‚ðŒp³‚·‚é
			if (node->left->symbol && node->left->symbol->type)
			{
				node->type = node->left->symbol->type;
			}
			// ¶•Ó‚ÌŒ^‚ª“o˜^‚³‚ê‚Ä‚¢‚é‚È‚çA¶•Ó‚ÌŒ^‚ðŒp³‚·‚é
			else if (node->left->type)
			{
				node->type = node->left->type;
			}
		}
	}
}

void mana_evaluator_evaluate(mana_node* node)
{
	if (node == NULL)
		return;

DO_RECURSIVE:

	switch (node->id)
	{
		// “Á‚Éˆ—‚ðs‚í‚È‚¢ƒm[ƒh
	case MANA_NODE_NEWLINE:
		MANA_ASSERT(node->body == NULL);
		mana_evaluator_evaluate(node->left);
		mana_evaluator_evaluate(node->right);
		break;

	case MANA_NODE_IDENTIFIER:
		mana_compiler_resolve_identifier(node);
		break;

		// ’è”’è‹`‚ÉŠÖ‚·‚éƒm[ƒh									
	case MANA_NODE_DEFINE_ALIAS:
		mana_symbol_create_alias(node->string, node->string);
		break;

	case MANA_NODE_DEFINE_CONSTANT:
		mana_symbol_create_const_int(node->string, node->digit);
		break;

	case MANA_NODE_UNDEFINE_CONSTANT:
		mana_symbol_destroy(node->string);
		break;

		// ƒƒ‚ƒŠƒŒƒCƒAƒEƒg‚ÉŠÖ‚·‚éƒm[ƒh									
	case MANA_NODE_DECLARE_ALLOCATE:
		{
			const int32_t mana_allocated_size = mana_symbol_get_static_memory_address() + node->digit;

			mana_evaluator_evaluate(node->left);

			const int32_t address = mana_symbol_get_static_memory_address();
			if (address >= mana_allocated_size)
			{
				mana_compile_error("static variable range over");
			}
			mana_symbol_set_static_memory_address(mana_allocated_size);
		}
		break;

	case MANA_NODE_DECLARE_STATIC:
		mana_evaluator_object.static_block_opend = true;
		mana_evaluator_evaluate(node->left);
		mana_evaluator_object.static_block_opend = false;
		break;

		// \‘¢‚ÉŠÖ‚·‚éƒm[ƒh
	case MANA_NODE_DECLARE_ACTOR:
		{
			mana_symbol_begin_registration_actor(mana_symbol_lookup(node->string));
			mana_evaluator_evaluate(node->left);
			mana_symbol_commit_registration_actor(node->string, NULL, NULL, false);
		}
		break;

	case MANA_NODE_DECLARE_EXTEND:
		mana_symbol_extend_module(node->string);
		break;

	case MANA_NODE_DECLARE_MODULE:
		{
			mana_symbol_begin_registration_module(mana_symbol_lookup(node->string));
			mana_evaluator_evaluate(node->left);
			mana_symbol_commit_registration_module(node->string);
		}
		break;

	case MANA_NODE_DECLARE_PHANTOM:
		{
			mana_symbol_begin_registration_actor(mana_symbol_lookup(node->string));
			mana_evaluator_evaluate(node->left);
			mana_symbol_commit_registration_actor(node->string, NULL, NULL, true);
		}
		break;

	case MANA_NODE_DECLARE_STRUCT:
		mana_symbol_open_structure();
		mana_evaluator_evaluate(node->left);
		mana_symbol_close_structure(node->string);
		break;

		// ŠÖ”éŒ¾‚ÉŠÖ‚·‚éƒm[ƒh									
	case MANA_NODE_DECLARE_ACTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			node->symbol = mana_symbol_create_function(node->string);
			node->symbol->type = mana_type_get(MANA_DATA_TYPE_VOID);
		}
		break;

	case MANA_NODE_DECLARE_ARGUMENT:
		MANA_ASSERT(node->body == NULL);
		mana_compiler_resolve_variable_description(node->left, MANA_MEMORY_TYPE_PARAMETER);
		mana_evaluator_evaluate(node->right);
		break;

	case MANA_NODE_DECLARE_FUNCTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			// ŠÖ”‚Ì–ß‚è’l‚ð•]‰¿
			mana_evaluator_evaluate(node->left);
			// ƒVƒ“ƒ{ƒ‹‚Ìì¬‚ÆŒ^‚Ì’è‹`
			node->symbol = mana_symbol_create_function(node->string);
			node->symbol->type = node->left->type;
			// ƒVƒ“ƒ{ƒ‹‚Éˆø”‚Ì”‚ð“o˜^
			node->symbol->number_of_parameters = mana_evaluator_get_argument_count(0, node->right);
		}
		break;

	case MANA_NODE_DECLARE_NATIVE_FUNCTION:
		{
			MANA_ASSERT(node->symbol == NULL);
			// ƒVƒ“ƒ{ƒ‹‚Ìì¬‚ÆŒ^‚Ì’è‹`
			node->symbol = mana_symbol_create_function(node->string);
			mana_symbol_begin_native_function_registration();
			mana_evaluator_evaluate(node->left);
			mana_evaluator_evaluate(node->right);
			node->symbol->number_of_parameters = mana_evaluator_get_argument_count(0, node->right);
			mana_symbol_commit_native_function_registration(node->symbol, node->left->type);
		}
		break;

		// •Ï”éŒ¾‚ÉŠÖ‚·‚éƒm[ƒh									
	case MANA_NODE_DECLARATOR:
		MANA_ASSERT(node->symbol == NULL);
		node->symbol = mana_symbol_create_identification(node->string, NULL, mana_evaluator_object.static_block_opend);
		break;

	case MANA_NODE_DECLARE_VARIABLE:
		mana_compiler_resolve_variable_description(node, MANA_MEMORY_TYPE_NORMAL);
		break;

	case MANA_NODE_TYPE_DESCRIPTION:
		mana_compiler_resolve_type_description(node);
		break;

	case MANA_NODE_VARIABLE_SIZE:
		MANA_ASSERT(node->left);
		MANA_ASSERT(node->right);
		if (node->string)
		{
			mana_symbol_entry* symbol = mana_symbol_lookup(node->string);
			if (symbol)
				node->digit = symbol->address;
			else
				mana_compiler_error("undefined symbol '%s'", node->string);
		}
		break;

		///////////////////////////////////////////////////////////////////////
		// ˆÈ‰º‚ÍƒVƒ“ƒ{ƒ‹¶¬‚ðs‚í‚È‚¢ƒm[ƒh
		///////////////////////////////////////////////////////////////////////

		// ƒuƒƒbƒN‚ð”º‚¤§Œä‚ÉŠÖ‚·‚éƒm[ƒh
	case MANA_NODE_BLOCK:
	case MANA_NODE_BREAK:
	case MANA_NODE_CASE:
	case MANA_NODE_CONTINUE:
	case MANA_NODE_DEFAULT:
	case MANA_NODE_DO:
	case MANA_NODE_FOR:
	case MANA_NODE_GOTO:
	case MANA_NODE_HALT:
	case MANA_NODE_IF:
	case MANA_NODE_LABEL:
	case MANA_NODE_LOCK:
	case MANA_NODE_LOOP:
	case MANA_NODE_RETURN:
	case MANA_NODE_ROLLBACK:
	case MANA_NODE_SWITCH:
	case MANA_NODE_WHILE:

		// •¡”‚Ì–½—ß‚É“WŠJ‚³‚ê‚éƒm[ƒh									
	case MANA_NODE_COMPLY:
	case MANA_NODE_JOIN:
	case MANA_NODE_MEMOP:
	case MANA_NODE_PRINT:
	case MANA_NODE_REFUSE:
	case MANA_NODE_REQUEST:
	case MANA_NODE_STRING:
	case MANA_NODE_VARIABLE:
	case MANA_NODE_YIELD:

		// ’†ŠÔŒ¾Œê‚Æ‘Î‚É‚È‚éƒm[ƒh
	case MANA_NODE_SIZEOF:
	case MANA_NODE_ARRAY:
	case MANA_NODE_ASSIGN:
	case MANA_NODE_CONST:
	case MANA_NODE_EXPRESSION_IF:
	case MANA_NODE_MEMBER_FUNCTION:
	case MANA_NODE_MEMBER_VARIABLE:
	case MANA_NODE_CALL:
	case MANA_NODE_CALL_ARGUMENT:
	case MANA_NODE_ADD:
	case MANA_NODE_SUB:
	case MANA_NODE_MUL:
	case MANA_NODE_DIV:
	case MANA_NODE_REM:
	case MANA_NODE_NEG:
	case MANA_NODE_POW:
	case MANA_NODE_NOT:
	case MANA_NODE_AND:
	case MANA_NODE_OR:
	case MANA_NODE_XOR:
	case MANA_NODE_LSH:
	case MANA_NODE_RSH:
	case MANA_NODE_LS:
	case MANA_NODE_LE:
	case MANA_NODE_EQ:
	case MANA_NODE_NE:
	case MANA_NODE_GE:
	case MANA_NODE_GT:
	case MANA_NODE_I2F:
	case MANA_NODE_F2I:
	case MANA_NODE_LOR:
	case MANA_NODE_LAND:
	case MANA_NODE_LNOT:
	case MANA_NODE_SENDER:
	case MANA_NODE_SELF:
	case MANA_NODE_PRIORITY:
	default:
		MANA_BUG("illigal node detect");
	}

	// Žqƒm[ƒh‚©‚çŒ^‚ðŒp³‚·‚é
	mana_evaluator_inherit_type_from_child_node(node);

	if (node->next)
	{
		// ––”öÄ‹A‚È‚Ì‚Ågoto‚É‚Äˆ—‚·‚é
		//mana_evaluator_evaluate(node->next);
		node = node->next;
		goto DO_RECURSIVE;
	}
}
