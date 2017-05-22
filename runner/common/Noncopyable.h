/*!
mana (compiler/library)

@author	Shun Moriya
@date	2017-
*/

#pragma once

namespace mana
{
	/*!
	non copyable class

	class Foo : private Noncopyable {};
	*/
    struct Noncopyable
    {
        Noncopyable() = default;
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator=(const Noncopyable&) = delete;
    };
}
