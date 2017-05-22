/*!
mana (compiler)

@author	Shun Moriya
@date	2003-
*/

#include "Path.h"
#if defined(MANA_TARGET_WINDOWS)
#include <cstdlib>
#include <direct.h>
#else
#include <unistd.h>
#endif

namespace mana
{
	void makepath(char* path, const size_t pathCount, const char* drive, const char* dir, const char* file, const char* ext)
	{
#if defined(MANA_TARGET_WINDOWS)
		_makepath_s(path, pathCount, drive, dir, file, ext);
#else
		strcpy(path, pathCount, drive);
		strcat(path, pathCount, dir);
		strcat(path, pathCount, file);
		strcat(path, pathCount, ext);
#endif
	}

#if !defined(MANA_TARGET_WINDOWS)
	/* range copy */
	static void rcopy(char* dp, const char* sp, const char* ep)
	{
		while(sp < ep)
			*dp ++ = *sp ++;
		*dp ++ = '\0';
	}

	/* reverse find */
	static const char* rfind(const char* sp, const char* ep, const char c)
	{
		while(*ep != c)
		{
			if(sp == ep)
				return nullptr;
			-- ep;
		}
		return ep;
	}
#endif

	void splitpath(const char* sptr, char* drive, const size_t driveCount, char* dir, const size_t dirCount, char* file, const size_t fileCount, char* ext, const size_t extCount)
	{
#if defined(MANA_TARGET_WINDOWS)
		_splitpath_s(sptr, drive, driveCount, dir, dirCount, file, fileCount, ext, extCount);
#else
		*drive = *dir = *file = *ext = '\0';
		const char* tptr  = sptr;
		const char* bptr = sptr + strlen(sptr);
		const char* aptr = strchr(sptr, ':');
		if(aptr)
		{
			rcopy(drive, sptr, aptr+1);
			sptr = aptr + 1;
		}

		tptr = rfind(tptr, bptr, '/');
		if(tptr)
		{
			aptr = rfind(tptr, bptr, '.');
			if(aptr)
			{
				rcopy(ext, aptr, bptr);
				bptr = aptr;
			}
			if(tptr < bptr)
				rcopy(file, tptr + 1, bptr);
			bptr = tptr;
		}
		else
		{
			tptr = sptr;
			aptr = rfind(tptr, bptr, '.');
			if(aptr)
			{
				rcopy(ext, aptr, bptr);
				bptr = aptr;
			}
			rcopy(file, tptr, bptr);
			bptr = tptr;
		}

		if(sptr < bptr)
		{
			rcopy(dir, sptr, bptr + 1);
		}
#endif
	}

	char* fullpath(char* out, const char* in, const size_t size)
	{
#if defined(MANA_TARGET_WINDOWS)
		return _fullpath(out, in, size);
#else
#if 0
		char* result = realpath(in, nullptr);
		if (result)
			strncpy(out, result, size);
		free(result);
		return out;
#else
		return realpath(in, out);
#endif
#endif
	}

	int chdir(const char* path)
	{
#if defined(MANA_TARGET_WINDOWS)
		return _chdir(path);
#else
		return ::chdir(path);
#endif
	}
}
