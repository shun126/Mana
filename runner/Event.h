/*!
mana (library)

@file	Event.h
@author	Shun Moriya
@date	2024-
@brief	Provides notification system

This system provides for dynamic addition and removal of notification functions.
*/

#pragma once
#include "common/Setup.h"
#include <functional>
#include <unordered_map>

namespace mana
{
	/**
	 * notification name of the registered event
	 */
	using EventNameType = uint32_t;

	/**
	 * Invalid notification name
	 */
	static constexpr EventNameType EventInvalidName = 0;

	/**
	 * @brief	Send notifications to registered functions
	 * @tparam Arguments	Arguments to Notification Functions
	 */
	template <typename... Arguments>
	class Event : Noncopyable
	{
	public:
		using FunctionType = void(Arguments...);

	public:
		Event() = default;
		virtual ~Event() = default;

		/**
		 * @brief	Add a notification function
		 * @param function	Notified Functions
		 * @return	notification function identifier
		 */
		[[nodiscard]] EventNameType Add(const std::function<FunctionType>& function)
		{
			static EventNameType index = 1;
			mFunction[index] = function;
			return index++;
		}

		/**
		 * @brief	Deletes the registration of a notification function.
		 * @param	name	notification function identifier
		 */
		void Remove(const EventNameType name)
		{
			mFunction.erase(name);
		}

		/**
		 * @brief 	Checks if a notification function is registered
		 * @param	name	notification function identifier
		 * @return	If true, then registered
		 */
		[[nodiscard]] bool Contain(const EventNameType name) const
		{
			return mFunction.contain(name);
		}

		/**
		 * @brief Send notifications to registered functions
		 * @param arguments Notification Function Arguments
		 */
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
