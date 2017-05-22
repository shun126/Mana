/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once

namespace mana
{
typedef struct register_entity
{
	SymbolEntry* symbol;
	size_t address;
	int8_t number;
}register_entity;

extern void register_initialzie(void);
extern void register_finalize(void);
extern void register_clear(void);

extern register_entity* register_find(SymbolEntry* symbol);

extern register_entity* register_allocate(SymbolEntry* symbol, size_t address);
extern void register_release(register_entity* entity);
}

