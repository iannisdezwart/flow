#ifndef FLOW_FILE_PROVIDER_HEADER
#define FLOW_FILE_PROVIDER_HEADER

#include "../data-structures/content-provider.hpp"
#include "file.hpp"

namespace flow {
	class FileProvider : public ContentProvider {
		FileReadStream& file_stream;

		public:
			FileProvider(FileReadStream& file_stream)
				: file_stream(file_stream), ContentProvider(file_stream.size()) {}

			String next_chunk(size_t offset, size_t desired_size)
			{
				return file_stream.read(desired_size);
			}
	};
};


#endif