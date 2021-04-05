#ifndef FLOW_FILE_PROVIDER_HEADER
#define FLOW_FILE_PROVIDER_HEADER

#include <bits/stdc++.h>

#include "../data-structures/content-provider.hpp"
#include "file-stream.hpp"

namespace flow {
	class FileProvider : public FileReadStream, public ContentProvider {
		public:
			FileProvider(const char *file_name)
				: FileReadStream(file_name),
					ContentProvider(FileReadStream::size()) {}

			String next_chunk(size_t offset, size_t desired_size)
			{
				return read(desired_size);
			}
	};
};


#endif