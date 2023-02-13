/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Platform.h"
#include "String.h"

#define MANA_WARNING(...)	{ mana::Trace(__VA_ARGS__); }
#define MANA_ERROR(...)		{ mana::Trace(__VA_ARGS__); std::terminate(); }
#define MANA_BUG(...)		{ mana::Trace({ __FILE__, "(", std::to_string(__LINE__), "): BUG!: ", __VA_ARGS__, "\n" }); std::terminate(); }
#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
#define MANA_CHECK(EXP)		if(!(EXP)){ mana::Trace({ #EXP, ": ", __FILE__, "(", std::to_string(__LINE__), ")\n" }); }
#define MANA_ASSERT(EXP)	if(!(EXP)){ mana::Trace({ #EXP, ": ", __FILE__, "(", std::to_string(__LINE__), ")\n" }); std::terminate(); }
#define MANA_ASSERT_MESSAGE(EXP, ...) if(!(EXP)){ mana::Trace({ #EXP, ": ", __FILE__, "(", std::to_string(__LINE__), "): ", __VA_ARGS__, "\n" }); std::terminate(); }
#else
#define MANA_CHECK(EXP)		((void)0)
#define MANA_ASSERT(EXP)	((void)0)
#define MANA_ASSERT_MESSAGE(EXP, ...) ((void)0)
#endif
#define MANA_VERIFY(EXP) \
if(!(EXP)){ \
	mana::Trace({ __FILE__, "(", std::to_string(__LINE__), "): (", #EXP, ")\n" }); \
	std::terminate(); \
}
#define MANA_VERIFY_MESSAGE(EXP, ...) \
if(!(EXP)){ \
	mana::Trace({ __FILE__, "(", std::to_string(__LINE__), "): ", __VA_ARGS__, " (", #EXP, ")\n" }); \
	std::terminate(); \
}
#define MANA_NOT_IMPLEMENTED() { \
	mana::Trace({ __FILE__, "(", std::to_string(__LINE__), "): NOT implemented!\n" }); \
	std::terminate(); \
}
