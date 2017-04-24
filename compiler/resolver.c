/*!
mana (compiler)

@file	resolver.c
@brief	ƒVƒ“ƒ{ƒ‹‚Ì‰ðŒˆ‚ÉŠÖŒW‚·‚éƒ\[ƒXƒtƒ@ƒCƒ‹
@detail	ƒVƒ“ƒ{ƒ‹‚Ì‰ðŒˆ‚ÉŠÖŒW‚·‚éƒ\[ƒXƒtƒ@ƒCƒ‹‚Å‚·B
@author	Shun Moriya
@date	2017-
*/

#if !defined(___MANA_RESOLVER_H___)
#include "resolver.h"
#endif
#if !defined(___MANA_ERROR_H___)
#include "error.h"
#endif
#if !defined(___MANA_LEXER_H___)
#include "lexer.h"
#endif
#if !defined(___MANA_TYPE_H___)
#include "type.h"
#endif

void resolver_set_current_file_infomation(node_entry* self)
{
	MANA_ASSERT(self);
	lexer_set_current_filename(self->filename);
	lexer_set_current_line(self->line);
}

bool resolver_search_symbol_from_name(node_entry* self)
{
	MANA_ASSERT(self);

	bool result = true;

	if (self->symbol == NULL)
	{
		self->symbol = symbol_lookup(self->string);
		if (self->symbol)
		{
			if (self->type == NULL)
				self->type = self->symbol->type;
		}
		else
		{
			mana_compile_error("incomplete type name '%s'", self->string);
			self->type = type_get(SYMBOL_DATA_TYPE_INT);
			result = false;
		}
	}

	return result;
}

void resolver_resolve_type_description(node_entry* self)
{
	MANA_ASSERT(self);

	if (self->type == NULL)
	{
		self->symbol = symbol_lookup(self->string);
		if (self->symbol)
		{
			self->type = self->symbol->type;
		}
		else
		{
			mana_compile_error("incomplete type name '%s'", self->string);
			self->type = type_get(SYMBOL_DATA_TYPE_INT);
		}
	}
}

void resolver_resolve_declarator(node_entry* self, const bool static_variable)
{
	MANA_ASSERT(self);

	if (self->symbol != NULL)
		return;

	type_description* type = NULL;
	if (self->left && self->left->id == NODE_VARIABLE_SIZE)
		type = resolver_resolve_variable_size(self->left);

	self->symbol = symbol_create_variable(self->string, type, static_variable);
}

type_description* resolver_resolve_variable_size(node_entry* self)
{
	if (self == NULL)
		return NULL;

	MANA_ASSERT(self->left == NULL);
	MANA_ASSERT(self->right == NULL);

	if (self->string)
	{
		symbol_entry* symbol = symbol_lookup(self->string);
		if (symbol)
		{
			if (symbol->class_type == SYMBOL_CLASS_TYPE_CONSTANT_INT)
			{
				self->type = type_create_array(symbol->address);
			}
			else
			{
				mana_compile_error("invalid size information on parameter");
			}
		}
		else
		{
			mana_compile_error("identifier %s is not defined", self->string);
		}
	}
	else
	{
		if (self->digit > 0)
			self->type = type_create_array(self->digit);
		else
			mana_compile_error("invalid size information on parameter");
	}

	if (self->type)
	{
		self->type->component = resolver_resolve_variable_size(self->left);
	}

	return self->type;
}

void resolver_resolve_variable_description(node_entry* self, const symbol_memory_type_id memory_type_id, const bool static_variable)
{
	MANA_ASSERT(self);
	MANA_ASSERT(self->left && self->left->id == NODE_TYPE_DESCRIPTION);
	MANA_ASSERT(self->right && self->right->id == NODE_DECLARATOR);

	resolver_resolve_type_description(self->left);				// NODE_TYPE_DESCRIPTION
	resolver_resolve_declarator(self->right, static_variable);	// NODE_DECLARATOR

	//if (self->right->symbol->class_type == SYMBOL_CLASS_TYPE_VARIABLE_LOCAL)
	symbol_allocate_memory(self->right->symbol, self->left->type, memory_type_id);
	//symbol_allocate_memory(self->right->symbol, self->left->type, MEMORY_TYPE_PARAMETER);
}

void resolver_resolve_type_from_child_node(node_entry* self)
{
	MANA_ASSERT(self);

	// Ž©•ª‚ÌŒ^‚ª“o˜^‚³‚ê‚Ä‚¢‚È‚¢
	if (self->type == NULL)
	{
		// ƒVƒ“ƒ{ƒ‹‚ª“o˜^‚³‚ê‚Ä‚¢‚é‚È‚çAƒVƒ“ƒ{ƒ‹‚ÌŒ^‚ðŒp³‚·‚é
		if (self->symbol && self->symbol->type)
		{
			self->type = self->symbol->type;
		}
		else if (self->left)
		{
			// ¶•Ó‚ÌŒ^‚ª“o˜^‚³‚ê‚Ä‚¢‚é‚È‚çA¶•Ó‚ÌŒ^‚ðŒp³‚·‚é
			if (self->left->type)
			{
				self->type = self->left->type;
			}
			// ¶•Ó‚ÌƒVƒ“ƒ{ƒ‹‚ª“o˜^‚³‚ê‚Ä‚¢‚é‚È‚çA¶•Ó‚ÌƒVƒ“ƒ{ƒ‹‚ÌŒ^‚ðŒp³‚·‚é
			else if (self->left->symbol && self->left->symbol->type)
			{
				self->type = self->left->symbol->type;
			}
		}
	}
}
