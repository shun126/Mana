/*!
mana (library)

@author	Shun Moriya
@date	2024-
*/

#pragma once
#include "common/Setup.h"
#include <exception>
#include <functional>
#include <map>

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
				// Event callbacks belong to the host application. A callback failure must not
				// escape the mana runtime boundary or be invoked again from fault cleanup.
				try
				{
					function.second(arguments...);
				}
				catch (const std::exception& e)
				{
					try
					{
						Trace(TraceLevel::Error, { "mana: event callback threw and was ignored: ", e.what(), "\n" });
					}
					catch (...)
					{
						// Trace handlers are host callbacks too; never let a reporting failure escape.
					}
				}
				catch (...)
				{
					try
					{
						Trace(TraceLevel::Error, "mana: event callback threw and was ignored\n");
					}
					catch (...)
					{
						// Trace handlers are host callbacks too; never let a reporting failure escape.
					}
				}
			}
		}

	private:
		std::map<EventNameType, std::function<FunctionType>> mFunction;
	};
}
