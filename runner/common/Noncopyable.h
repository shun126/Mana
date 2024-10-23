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

	class Foo : Noncopyable {};
	*/
    struct Noncopyable
    {
        Noncopyable() = default;
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable(Noncopyable&&) noexcept = delete;
        ~Noncopyable() = default;

    	Noncopyable& operator=(const Noncopyable&) = delete;
        Noncopyable& operator=(Noncopyable&&) noexcept = delete;
    };
}
