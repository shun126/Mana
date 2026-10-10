/*!
 * mana (test)
 *
 * @author	Shun Moriya
 * @date	2026-
 */

// ProgramImageTest.cpp also includes ProgramImage.h. Linking both translation
// units into one executable detects functions in ProgramImage.inl that are
// defined without inline.
#include "../../runner/ProgramImage.h"

bool IsProgramImageLoadedInAnotherTranslationUnit(const mana::ProgramImage& image)
{
	return image.IsLoaded();
}
