/*!
mana (library)

@author	Shun Moriya
@date	2024-
*/

#pragma once
#include "common/Setup.h"
#include <functional>
#include <unordered_map>

namespace mana
{
	using EventNameType = uint32_t;
	static constexpr EventNameType EventInvalidName = 0;

	template <typename... Arguments>
	class Event : Noncopyable
	{
	public:
		using FunctionType = void(Arguments...);

	public:
		Event() = default;
		virtual ~Event() = default;

		[[nodiscard]] EventNameType Add(const std::function<FunctionType>& function)
		{
			static EventNameType index = 1;
			mFunction[index] = function;
			return index++;
		}

		void Remove(const EventNameType name)
		{
			mFunction.erase(name);
		}

		[[nodiscard]] bool Contain(const EventNameType name) const
		{
			return mFunction.contain(name);
		}

		void Broadcast(Arguments... arguments) const
		{
			for (const auto& function : mFunction)
			{
				function.second(arguments...);
			}
		}

	private:
		std::unordered_map<EventNameType, std::function<FunctionType>> mFunction;
	};
}
