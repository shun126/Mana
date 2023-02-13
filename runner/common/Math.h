/*!
mana (library)

@author	Shun Moriya
@date	2003-
*/

#pragma once
#include "Platform.h"
#include "Noncopyable.h"
#include <random>

namespace mana
{
	class SystemRandom : private Noncopyable
	{
	public:
		static SystemRandom& GetInstance()
		{
			static SystemRandom instance;
			return instance;
		}

		std::mt19937_64& Get()
		{
			return systemRandom;
		}

	private:
		std::mt19937_64 systemRandom;
	};

	inline void srand(const int_t seed)
	{
		SystemRandom::GetInstance().Get().seed(seed);
	}

	inline int_t irand()
	{
		std::uniform_int_distribution<int_t> range;
		return range(SystemRandom::GetInstance().Get());
	}

	inline float_t frand()
	{
		std::uniform_real_distribution<float_t> range;
		return range(SystemRandom::GetInstance().Get());
	}

	template<typename T>
	const T Pi() { return static_cast<T>(3.1415926535897932384626433832795028841971693993751); }

	template<typename T>
	T ToRadian(T degree) { return degree * (Pi<T>() / static_cast<T>(180.0)); }

	template<typename T>
	T ToDegree(T radian) { return radian * (static_cast<T>(180.0) / Pi<T>()); }

	template <typename T>
	T AngleMod(const T x, const T div)
	{
		const T div2 = div * static_cast<T>(2);
		return x - static_cast<intptr_t>(((x >= 0) ? (x + div) : (x - div)) / div2) * div2;
	}

	template <typename T>
	inline T Alignment(const T value, const T alignment)
	{
		return (value + alignment - 1) / alignment;
	}

	template <typename T>
	inline T AlignUp(const T value, const T alignment)
	{
		return Alignment(value, alignment) * alignment;
	}
}
