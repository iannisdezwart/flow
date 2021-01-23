#ifndef EVENT_EMITTER_HEADER
#define EVENT_EMITTER_HEADER

#include <bits/stdc++.h>
#include "../flow.hpp"

using namespace std;
using namespace flow;

namespace flow {
	typedef size_t event_id_t;

	template <typename... Args>
	struct EventListener {
		function<void(Args...)> callback;
		event_id_t id;
		bool recurrent;
	};

	/**
	 *  @brief  Event handling class. Listeners can be added and triggered.
	 */
	template <typename... Args>
	class EventEmitter {
		private:
			DynamicArray<EventListener<Args...>> listeners;
			event_id_t current_id = 0;

		public:
			/**
			 *  @brief  Adds a listener to the EventEmitter.
			 *  @param  callback  Is called when the event is triggered.
			 *  @param  recurrent  Boolean that indicates whether this listener
			 *  should keep on existing after it has been triggered one time.
			 *  @returns  The id of this listener. You will need to pass this to
			 *  EventEmitter::remove_listener() if you wish to delete the listener.
			 */
			event_id_t add_listener(function<void(Args...)> callback,
				bool recurrent = true)
			{
				event_id_t id = current_id++;

				EventListener<Args...> listener = { callback, id, recurrent };
				listeners.append(listener);

				return id;
			}

			/**
			 *  @brief  Removes a listener from this EventEmitter.
			 *  @param  listener_id  The id returned upon adding a listener with
			 *  EventEmitter::add_listener().
			 *  @returns  A boolean indicating whether the listener has been found
			 *  and got removed.
			 */
			bool remove_listener(size_t listener_id)
			{
				bool found = false;
				size_t listeners_size = listeners.size();

				for (size_t i = 0; i < listeners_size; i++) {
					if (listeners[i].id == listener_id && i != listeners_size - 1) {
						found = true;
						listeners[i] = listeners[listeners_size - 1];
					}
				}

				listeners.extract_rear();
				return found;
			}

			/**
			 *  @brief  Triggers all currently existing listeners with the
			 *  user defined argument template. The order of the triggered
			 *  listeners is undefined if EventEmitter::remove_listener(...) was
			 *  called OR EventEmitter::add_listener(..., false) was called.
			 */
			void trigger(Args... args)
			{
				size_t i = 0;

				while (i < listeners.size()) {
					EventListener<Args...>& listener = listeners[i];
					listener.callback(args...);

					if (listener.recurrent) {
						i++;
					} else {
						if (i != listeners.size() - 1) {
							listeners[i] = listeners[listeners.size() - 1];
						}

						listeners.extract_rear();
					}
				}
			}

			/**
			 *  @brief  Returns the amount of active listeners to the EventEmitter.
			 */
			size_t size()
			{
				return listeners.size();
			}
	};
};

#endif