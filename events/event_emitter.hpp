#ifndef EVENT_EMITTER_HEADER
#define EVENT_EMITTER_HEADER

#include <bits/stdc++.h>
#include "../flow.hpp"

using namespace std;
using namespace flow;

namespace flow {
	template <typename... Args>
	struct EventListener {
		function<void(Args...)> callback;
		size_t id;
	};

	template <typename... Args>
	class EventEmitter {
		private:
			DynamicArray<EventListener<Args...>> listeners;
			size_t current_id = 0;

		public:
			size_t add_listener(function<void(Args...)> callback)
			{
				size_t id = current_id++;

				EventListener<Args...> listener = { callback, id };
				listeners.append(listener);

				return id;
			}

			void remove_listener(size_t listener_id)
			{
				size_t listeners_size = listeners.size();

				for (size_t i = 0; i < listeners_size; i++) {
					if (listeners[i].id == listener_id && i != listeners_size - 1) {
						listeners[i] = listeners[listeners_size - 1];
					}
				}

				listeners.extract_rear();
			}

			void trigger(Args... args)
			{
				for (size_t i = 0; i < listeners.size(); i++) {
					EventListener<Args...>& listener = listeners[i];
					listener.callback(args...);
				}
			}
	};
};

#endif