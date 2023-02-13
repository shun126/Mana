/*!
mana (compiler/library)

@author	Shun Moriya
@date	2003-
*/

#pragma once

#if !defined(MANA_TARGET_DETECTED)
	#if defined(_WIN32) || defined(_WIN64)
		#define MANA_TARGET_WINDOWS (1)
		#if defined(_WIN64)
			#define MANA_TARGET_WINDOWS_64 (1)
		#elif defined(_WIN32)
			#define MANA_TARGET_WINDOWS_32 (1)
		#else
			#error Platform Not Supported.
		#endif
	#elif defined(__APPLE__)
		#define MANA_TARGET_APPLE (1)
		#if defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
			#define MANA_TARGET_IPHONE (1)
			#define MANA_TARGET_EMBEDDED (1)
		#elif defined(__MAC_OS_X_VERSION_MIN_REQUIRED)
			#define MANA_TARGET_MAC_OSX (1)
		#else
			#error Platform Not Supported.
		#endif
	#elif defined(__unix__)
		#define MANA_TARGET_UNIX (1)
		#if defined(__GNUC__)
		#endif
	#else
		#error Platform Not Supported.
	#endif
	#define MANA_TARGET_DETECTED
#endif

#define MANA_BUILD_DEBUG (1)
#define MANA_BUILD_RELEASE (2)
#if defined(_DEBUG) || defined(DEBUG)
#define MANA_BUILD_TARGET MANA_BUILD_DEBUG
#else
#define MANA_BUILD_TARGET MANA_BUILD_RELEASE
#endif

#define MANA_UNUSED_VAR(var)	((void)&var)
#define MANA_ELEMENT_OF(var)	(sizeof(var) / sizeof(var[0]))

#include "Type.h"
