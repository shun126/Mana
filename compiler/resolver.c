/*
mana (compiler)

@file	mana_resolver.c
@brief	ƒVƒ“ƒ{ƒ‹‚Ì‰ðŒˆ‚ÉŠÖŒW‚·‚éƒ\[ƒXƒtƒ@ƒCƒ‹
@detail	ƒVƒ“ƒ{ƒ‹‚Ì‰ðŒˆ‚ÉŠÖŒW‚·‚éƒ\[ƒXƒtƒ@ƒCƒ‹‚Å‚·B
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_RESOLVER_H___)
#include "resolver.h"
#endif

void mana_resolver_search_symbol_from_name(mana_node* node)
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

void mana_resolver_resolve_type_description(mana_node* node)
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

void mana_resolver_resolve_declarator(mana_node* node, const bool static_variable)
{
	MANA_ASSERT(node);

	if (node->symbol != NULL)
		return;

	mana_type_description* type = NULL;
	if (node->left && node->left->id == MANA_NODE_VARIABLE_SIZE)
		type = mana_resolver_resolve_variable_size(node->left);

	node->symbol = mana_symbol_create_identification(node->string, type, false);
}

mana_type_description* mana_resolver_resolve_variable_size(mana_node* node)
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
		node->type->component = mana_resolver_resolve_variable_size(node->left);
	}

	return node->type;
}

void mana_resolver_resolve_variable_description(mana_node* node, const mana_symbol_memory_type_id memory_type_id, const bool static_variable)
{
	MANA_ASSERT(node);
	MANA_ASSERT(node->left && node->left->id == MANA_NODE_TYPE_DESCRIPTION);
	MANA_ASSERT(node->right && node->right->id == MANA_NODE_DECLARATOR);

	mana_resolver_resolve_type_description(node->left);				// MANA_NODE_TYPE_DESCRIPTION
	mana_resolver_resolve_declarator(node->right, static_variable);	// MANA_NODE_DECLARATOR

	//if (node->right->symbol->class_type == MANA_CLASS_TYPE_VARIABLE_LOCAL)
	mana_symbol_allocate_memory(node->right->symbol, node->left->type, memory_type_id);
	//mana_symbol_allocate_memory(node->right->symbol, node->left->type, MANA_MEMORY_TYPE_PARAMETER);
}

void mana_resolver_resolve_type_from_child_node(mana_node* node)
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
			// ¶•Ó‚ÌŒ^‚ª“o˜^‚³‚ê‚Ä‚¢‚é‚È‚çA¶•Ó‚ÌŒ^‚ðŒp³‚·‚é
			if (node->left->type)
			{
				node->type = node->left->type;
			}
			// ¶•Ó‚ÌƒVƒ“ƒ{ƒ‹‚ª“o˜^‚³‚ê‚Ä‚¢‚é‚È‚çA¶•Ó‚ÌƒVƒ“ƒ{ƒ‹‚ÌŒ^‚ðŒp³‚·‚é
			else if (node->left->symbol && node->left->symbol->type)
			{
				node->type = node->left->symbol->type;
			}
		}
	}
}
