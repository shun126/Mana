/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Platform.h"
#include "String.h"

#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
#define MANA_WARNING(...)	{ mana::Trace(__VA_ARGS__); }
#define MANA_ERROR(...)		{ mana::Trace(__VA_ARGS__); abort(); }
#define MANA_CHECK(EXP)		if(!(EXP)){ mana::Trace("%s: %s(%d)\n", #EXP, __FILE__, __LINE__); }
#define MANA_ASSERT(EXP)	if(!(EXP)){ mana::Trace("%s: %s(%d)\n", #EXP, __FILE__, __LINE__); abort(); }
#define MANA_ASSERT_MESSAGE(EXP, m) if(!(EXP)){ mana::Trace("%s: %s(%d): %s\n", #EXP, __FILE__, __LINE__, m); abort(); }
#define MANA_BUG(m) { mana::Trace("%s(%d): BUG!: %s\n", __FILE__, __LINE__, m); abort(); }
#else
#define MANA_WARNING(...)	{ printf(__VA_ARGS__); }
#define MANA_ERROR(...)		{ printf(__VA_ARGS__); abort(); }
#define MANA_CHECK(EXP)		((void)0)
#define MANA_ASSERT(EXP)	((void)0)
#define MANA_ASSERT_MESSAGE(EXP, m) ((void)0)
#define MANA_BUG(m) { printf("%s(%d): BUG!: %s\n", __FILE__, __LINE__, m); abort(); }
#endif
#define MANA_VERIFY(EXP) \
if(!(EXP)){ \
	MANA_PRINT("%s(%d): (%s)\n", __FILE__, __LINE__, #EXP); \
	abort(); \
}
#define MANA_VERIFY_MESSAGE(EXP, ...) \
if(!(EXP)){ \
	MANA_PRINT("%s(%d): ", __FILE__, __LINE__); \
	MANA_PRINT(__VA_ARGS__); \
	MANA_PRINT(" (%s)\n", #EXP); \
	abort(); \
}
#define MANA_NOT_IMPLEMENTED() { \
	mana::Trace("%s(%d): NOT implemented!\n", __FILE__, __LINE__); \
	abort(); \
}
