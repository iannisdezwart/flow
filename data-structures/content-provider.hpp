#ifndef FLOW_CONTENT_PROVIDER_HEADER
#define FLOW_CONTENT_PROVIDER_HEADER

#include "stream.hpp"
#include "string.hpp"

namespace flow {
	/**
	 *  @brief  A ContentProvider provides chunks of data on demand.
	 *  The virtual method next_chunk() must be implemented.
	 */
	class ContentProvider {
		public:
			size_t size;
			size_t bytes_provided = 0;

			bool finished = false;

			/**
			 *  @brief  Creates a ContentProvider instance.
			 */
			ContentProvider(size_t size) : size(size) {}

			/**
			 *  @brief  Abstract method that must be implemented by the developer.
			 *  It should return a chunk of the content, at any given offset.
			 *  The returned chunk should be no more than the given desired_size.
			 *  @param  offset  The offset to the content. This represents the
			 *  position of the first byte of the wanted chunk in the content.
			 *  @param  desired_size  The desired size for the chunk. Do not provide
			 *  a chunk larger than this size. If it is not possible to provide a
			 *  chunk of this size, it is okay to provide a chunk smaller than
			 *  the desired_size, or you could implement some buffering.
			 */
			virtual String next_chunk(size_t offset, size_t desired_size) = 0;

			/**
			 *  @brief  Writes a chunk of a given desired_size to a given stream.
			 *  @param  stream  The stream to write to.
			 *  @param  desired_size  The desired chunk size.
			 */
			void provide(Stream<String&>& stream, size_t desired_size)
			{
				String chunk = next_chunk(bytes_provided, desired_size);
				bytes_provided += chunk.size();

				if (bytes_provided >= size) finished = true;

				stream.write(chunk);
			}
	};
};

#endif