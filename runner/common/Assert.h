/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Platform.h"
#include "String.h"

#define MANA_WARNING(...)	(void)(mana::Trace(__VA_ARGS__))
#define MANA_ERROR(...)		(void)(mana::Trace(__VA_ARGS__), std::terminate())
#define MANA_BUG(...)		(void)(mana::Trace({ __FILE__, "(", std::to_string(__LINE__), "): BUG!: ", __VA_ARGS__, "\n" }), std::terminate())
#if MANA_BUILD_TARGET == MANA_BUILD_DEBUG
#define MANA_CHECK(expression) (void)( \
	(!!(expression)) || \
	(mana::Trace({ #expression, ": ", __FILE__, "(", std::to_string(__LINE__), ")\n" }), 0) \
	)
#define MANA_ASSERT(expression) (void)( \
	(!!(expression)) || \
	(mana::Trace({ #expression, ": ", __FILE__, "(", std::to_string(__LINE__), ")\n" }), std::terminate(), 0) \
	)
#define MANA_ASSERT_MESSAGE(expression, ...) (void)( \
	(!!(expression)) || \
	(mana::Trace({ #expression, ": ", __FILE__, "(", std::to_string(__LINE__), "): ", __VA_ARGS__, "\n" }), std::terminate(), 0) \
	)
#else
#define MANA_CHECK(expression)	((void)0)
#define MANA_ASSERT(expression)	((void)0)
#define MANA_ASSERT_MESSAGE(expression, ...) ((void)0)
#endif
#define MANA_VERIFY(expression)	(void)( \
	(!!(expression)) || \
	(mana::Trace({ __FILE__, "(", std::to_string(__LINE__), "): (", #expression, ")\n" }), std::terminate(), 0) \
	)
#define MANA_VERIFY_MESSAGE(expression, ...) (void)( \
	(!!(expression)) || \
	(mana::Trace({ __FILE__, "(", std::to_string(__LINE__), "): ", __VA_ARGS__, " (", #expression, ")\n" }), std::terminate(), 0) \
	)
#define MANA_NOT_IMPLEMENTED() (void)( \
	(mana::Trace({ __FILE__, "(", std::to_string(__LINE__), "): Not implemented!\n" }), std::terminate()) \
	)
