#pragma once

#ifndef CALLBACK_H
#define CALLBACK_H
#endif // !CALLBACK_H

#include <vector>
#include <functional>

namespace GEngine
{
	namespace Callbacks
	{
		typedef std::function<void()> cb_0param;
		//typedef std::function<void(uint32_t)> cb_1param;

		enum Event
		{
			NETWORKING_CLIENT_READY = 0,
			NETWORKING_CLIENT_CONNECT_SUCCESSFUL,
			NETWORKING_CLIENT_CONNECT_UNSUCCESSFUL,
			NETWORKING_CLIENT_DISCONNECTED,
			NETWORKING_SERVER_READY,
			NETWORKING_SERVER_SHUTDOWN,

			GAME_QUIT,
			MAX
		};

		struct EventCallback
		{
			// the callback
			std::function<void()> cb;
			Event event;
		};

		class EventDriver
		{
		public:
			static inline EventDriver& Instance()
			{
				static EventDriver instance;
				return instance;
			}

			void RegisterCallback(const cb_0param& cb, const Event event)
			{
				// add callback to end of callback list
				callbacks.push_back({ cb, event });
			}

			/// Call all the registered callbacks.
			void CallEvent(Event event) const
			{
				// iterate through callback list and call each one
				for (const auto& cb : callbacks)
				{
					if (cb.event == event)
					{
						cb.cb();
					}
				}
			}

			void RemoveCallback(std::function<void()> callback)
			{
				for (int i = 0; i < callbacks.size(); i++)
				{
					if (&callbacks.at(i).cb == &callback)
					{
						callbacks.erase(callbacks.begin() + i);
					}
				}
			}

		private:
			EventDriver() : callbacks() {}
			/// List of callback functions.
			std::vector<EventCallback> callbacks;
		};
	}
}