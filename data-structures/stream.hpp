#ifndef FLOW_STREAM_HEADER
#define FLOW_STREAM_HEADER

#include "../data-structures/dynamic-array.hpp"
#include "../events/event_emitter.hpp"

namespace flow {
	template <typename type>
	class Stream {
		public:
			bool active = false;

			DynamicArray<Stream<type> *> piped_streams;

			EventEmitter<> start_event;
			EventEmitter<> end_event;
			EventEmitter<type> write_event;
			EventEmitter<Stream<type> *> pipe_event;

			/**
			 *  @brief  Starts the Stream. After this method is called, the Stream
			 *  will become active, and data can be written to the Stream by calling
			 *  Stream::write().
			 */
			void start()
			{
				active = true;
				start_event.trigger();
			}

			/**
			 *  @brief  Ends the Stream. After this method is called, the Stream
			 *  will become inactive, and data can no longer be written to the
			 *  Stream by calling Stream::write().
			 */
			void end()
			{
				active = false;
				end_event.trigger();
			}

			/**
			 *  @brief  Writes one instance of data to the Stream.
			 *  @param  data  The data to write to the Stream.
			 */
			void write(type data)
			{
				if (!active) return;

				write_event.trigger(data);

				// Write to piped Streams

				for (size_t i = 0; i < piped_streams.size(); i++) {
					piped_streams[i]->write(data);
				}
			}

			/**
			 *  @brief  Pipes this Stream to another Stream.
			 *  All data written to this Stream will then also flow into that
			 *  piped Stream.
			 *  @param  stream  The Stream to pipe this Stream to.
			 */
			void pipe(Stream<type>& stream)
			{
				piped_streams.append(&stream);
				stream.pipe_event.trigger(this);
			}

			/**
			 *  @brief  Unpipes this Stream from another Stream.
			 *  Data written to this Stream will no longer flow into that Stream.
			 *  @param  stream  The Stream to unpipe.
			 */
			void unpipe(Stream<type>& stream)
			{
				for (size_t i = 0; i < piped_streams.size(); i++) {
					if (piped_streams[i] == &stream) {
						piped_streams[i] = piped_streams[piped_streams.size() - 1];
						piped_streams.extract_rear();
						i--;
					}
				}
			}
	};
};

#endif