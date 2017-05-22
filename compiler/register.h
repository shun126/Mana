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
	Symbol* symbol;
	size_t address;
	int8_t number;
}register_entity;

extern void register_initialzie(void);
extern void register_finalize(void);
extern void register_clear(void);

extern register_entity* register_find(Symbol* symbol);

extern register_entity* register_allocate(Symbol* symbol, size_t address);
extern void register_release(register_entity* entity);
}

