/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "../runner/common/Setup.h"
#if defined(MANA_TARGET_WINDOWS)
#endif

#if !defined(_MAX_PATH)
#define _MAX_PATH 260
#endif

#if !defined(_MAX_DRIVE)
#define _MAX_DRIVE 3
#endif

#if !defined(_MAX_DIR)
#define _MAX_DIR 256
#endif

#if !defined(_MAX_FNAME)
#define _MAX_FNAME 256
#endif

#if !defined(_MAX_EXT)
#define _MAX_EXT 256
#endif

namespace mana
{
	/*!
	Creates a path name from components.
	*/
	extern void makepath(char* path, const size_t pathCount, const char* drive, const char* dir, const char* file, const char* ext);

	/*!
	Break a path name into components. 
	*/
	extern void splitpath(const char* sptr, char* drive, const size_t driveCount, char* dir, const size_t dirCount, char* file, const size_t fileCount, char* ext, const size_t extCount);

	/*!
	return the canonicalized absolute pathname
	*/
	extern char* fullpath(char* out, const char* in, const size_t size);

	/*!
	change working directory
	*/
	extern int chdir(const char* path);

	/*!
	return path separator charactor
	*/
#if defined(MANA_TARGET_WINDOWS)
	constexpr char PathSeparator() { return '\\'; }
#else
	constexpr char PathSeparator() { return '/'; }
#endif
}
