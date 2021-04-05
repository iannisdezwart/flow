#ifndef FLOW_FILE_HEADER
#define FLOW_FILE_HEADER

#include <bits/stdc++.h>

#include "../data-structures/string.hpp"
#include "../data-structures/stream.hpp"
#include <cstdio>
#include <cstdlib>

namespace flow_file_tools {
	enum class FileMode { READ, WRITE, APPEND };
	enum class FileErrors { DOES_NOT_EXIST };
};

namespace flow {
	using namespace flow_file_tools;

	/**
	 *  @brief  An abstract wrapper around C-style FILE.
	 */
	template <enum FileMode Mode>
	class FileStream {
		protected:
			FILE *file;

			// template <typename = typename std::enable_if<Read>::type>
			size_t bytes_read = 0;
			// template <typename = typename std::enable_if<Write || Append>::type>
			size_t bytes_written = 0;

		public:
			// template <typename = typename std::enable_if<Read>::type>
			Stream<String&> read_stream;
			// template <typename = typename std::enable_if<Write || Append>::type>
			Stream<String&> write_stream;

			/**
			 *  @brief  Creates a FileStream.
			 */
			FileStream(const char *file_name)
			{
				// Open file in the correct mode

				switch (Mode) {
					case FileMode::READ:
						file = std::fopen(file_name, "r");
						if (file == NULL) throw FileErrors::DOES_NOT_EXIST;
						break;

					case FileMode::WRITE:
						file = std::fopen(file_name, "w+");
						break;

					case FileMode::APPEND:
						file = std::fopen(file_name, "a+");
						break;
				}

				// Open the read stream

				if (Mode == FileMode::READ) {
					read_stream.on_data([this](String& str) {
						bytes_read += str.size();
					});

					read_stream.start();
				}

				// Open the write stream

				if (Mode == FileMode::WRITE || Mode == FileMode::APPEND) {
					write_stream.on_data([this](String& str) {
						bytes_written += str.size();
						std::fwrite(str.begin(), 1, str.size(), file);
					});

					write_stream.start();
				}
			}

			/**
			 *  @brief  Creates a FileStream.
			 */
			FileStream(String& file_name) : FileStream(file_name.to_char_arr()) {}

			/**
			 *  @brief  Destructs a FileStream.
			 */
			~FileStream()
			{
				fclose(file);
			}

			/**
			 *  @brief  Returns the size of the file.
			 */
			size_t size()
			{
				// Save current file pointer location

				size_t current_offset = std::ftell(file);

				// Move to EOF and compute the size

				std::fseek(file, 0, SEEK_END);
				size_t size = std::ftell(file);

				// Move back to the original file pointer location

				std::fseek(file, current_offset, SEEK_SET);
				return size;
			}

			/**
			 *  @brief  Reads size bytes from the FileStream, stored in a String.
			 *  This String is written to FileStream::read_stream and also returned.
			 *  @param  size  The size to read.
			 */
			template <bool T = true>
			typename std::enable_if<(T && Mode == FileMode::READ), String>::type
			/* String */ read(size_t size)
			{
				// Read a chunk

				String chunk = String::alloc(size);
				size_t n = std::fread(chunk.begin(), 1, size, file);
				chunk.unsafe_set_element_count(n);

				// Write the chunk to the read_stream

				read_stream.write(chunk);
				return chunk;
			}

			/**
			 *  @brief  Writes a String into the FileStream.
			 *  Alias for FileStream::write_stream::write().
			 *  @param  str  The String to write.
			 */
			template <bool T = true>
			typename std::enable_if<T && Mode == FileMode::WRITE
				|| Mode == FileMode::APPEND, void>::type
			/* void */ write(String& str)
			{
				write_stream.write(str);
			}
	};

	/**
	 *  @brief  A read-only wrapper around C-style FILE.
	 *  Starts at the beginning of a file.
	 */
	class FileReadStream : public FileStream<FileMode::READ> {
		public:
			FileReadStream(const char *file_path) : FileStream(file_path) {}
			FileReadStream(String& file_path) : FileStream(file_path) {}
	};

	/**
	 *  @brief  A write-only wrapper around C-style FILE.
	 *  Overwrites a file.
	 */
	class FileWriteStream : public FileStream<FileMode::WRITE> {
		public:
			FileWriteStream(const char *file_path) : FileStream(file_path) {}
			FileWriteStream(String& file_path) : FileStream(file_path) {}
	};

	/**
	 *  @brief  A write-only wrapper around C-style FILE.
	 *  Appends to a file.
	 */
	class FileAppendStream : public FileStream<FileMode::APPEND> {
		public:
			FileAppendStream(const char *file_path) : FileStream(file_path) {}
			FileAppendStream(String& file_path) : FileStream(file_path) {}
	};
};

#endif