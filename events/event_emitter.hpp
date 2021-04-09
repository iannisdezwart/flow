#ifndef EVENT_EMITTER_HEADER
#define EVENT_EMITTER_HEADER

#include <bits/stdc++.h>

#include "../data-structures/dynamic-array.hpp"
#include "../data-structures/string.hpp"

namespace flow {
	typedef size_t event_id_t;

	template <typename... Args>
	class EventListener {
		private:
			typedef std::function<void(Args...)> callback_t;

		public:
			std::function<void(Args...)> callback;
			event_id_t id;
			bool recurrent;

			EventListener() {}

			EventListener(callback_t&& callback, event_id_t id,	bool recurrent)
				: callback(std::move(callback)), id(id), recurrent(recurrent) {}

			EventListener(const EventListener<Args...>& other)
				: callback(callback), id(id), recurrent(recurrent) {}

			EventListener(EventListener<Args...>&& other)
				: callback(std::move(callback)), id(id), recurrent(recurrent) {}

			EventListener<Args...>& operator=(const EventListener<Args...>& other)
			{
				if (this == &other) return *this;

				callback = other.callback;
				id = other.id;
				recurrent = other.recurrent;

				return *this;
			}

			EventListener<Args...>& operator=(EventListener<Args...>&& other)
			{
				if (this == &other) return *this;

				callback = std::move(other.callback);
				id = other.id;
				recurrent = other.recurrent;

				other.callback = NULL;

				return *this;
			}
	};

	/**
	 *  @brief  Event handling class. Listeners can be added and triggered.
	 */
	template <typename... Args>
	class EventEmitter {
		private:
			typedef std::function<void(Args...)> callback_t;
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
			event_id_t add_listener(callback_t&& callback, bool recurrent = true)
			{
				event_id_t id = current_id++;
				listeners.append(EventListener<Args...>(std::move(callback), id, recurrent));
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

					// We must save the recurrent flag in case the callback calls
					// EventEmitter::remove_listener() or EventEmitter::add_listener().
					// This would alter the listeners DynamicArray and maybe also the
					// reference, making it invalid or cause undefined behaviour.

					bool recurrent = listener.recurrent;

					// Fire the callback now

					listener.callback(args...);

					// And clean up this EventListener if it is not recurrent

					if (recurrent) {
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