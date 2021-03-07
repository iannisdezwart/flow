#ifndef FLOW_SOCKET_HEADER
#define FLOW_SOCKET_HEADER

#include "../data-structures/stream.hpp"
#include "../data-structures/string.hpp"

#ifndef FLOW_SOCKET_READ_BUFFER_SIZE
#define FLOW_SOCKET_READ_BUFFER_SIZE (size_t) 4096
#endif

#ifndef FLOW_SOCKET_WRITE_BUFFER_SIZE
#define FLOW_SOCKET_WRITE_BUFFER_SIZE (size_t) 4096
#endif

#ifndef FLOW_SOCKET_WRITE_BUFFER_RELEASE_SIZE
#define FLOW_SOCKET_WRITE_BUFFER_RELEASE_SIZE (size_t) 65536
#endif

namespace flow_socket_tools {
	enum class SocketReadingStates { READING, END };
	enum class SocketWritingStates { IDLE, WRITING };
};

namespace flow {
	using namespace flow_socket_tools;

	namespace net {
		#include <sys/types.h>
		#include <sys/socket.h>
		#include <sys/ioctl.h>
		#include <netinet/in.h>
		#include <unistd.h>
		#include <fcntl.h>
		#include <errno.h>

		constexpr in_addr_t inaddr_any = 0;

		ssize_t read(int socket_fd, String& dest)
		{
			return read(socket_fd, dest.begin(), dest.current_capacity());
		}

		ssize_t write(int socket_fd, const String& src)
		{
			return write(socket_fd, src.begin(), src.size());
		}

		int set_nonblocking(int fd)
		{
			int flags;

			#ifdef O_NONBLOCK
			if ((flags = fcntl(fd, F_GETFL, 0)) == -1) flags = 0;
			return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
			#else
			flags = 1;
			return ioctl(fd, FIONBIO, &flags);
			#endif
		}
	};

	class Socket {
		private:
			enum SocketReadingStates reading_state = SocketReadingStates::READING;
			enum SocketWritingStates writing_state = SocketWritingStates::IDLE;

			String reading_buffer;

			String *writing_buffer;
			size_t writing_buffer_offset;

			Stream<String&> out_chunks;

			void io_handle_read()
			{
				if (reading_state == SocketReadingStates::END) return;

				// Read a chunk

				ssize_t bytes_rw = net::read(socket_fd, reading_buffer);
				reading_buffer.unsafe_set_element_count(bytes_rw);

				if (bytes_rw < 0) {
					if (errno != EWOULDBLOCK && errno != EAGAIN)
						String::format("read() error %ld", bytes_rw).print();

					return;
				}

				// On reading end, end the reading state

				if (bytes_rw < FLOW_SOCKET_READ_BUFFER_SIZE) {
					reading_state = SocketReadingStates::END;
				}

				String::format("writing %lld bytes to socket input",
					reading_buffer.size()).print();

				in.write(reading_buffer);
			}

			void io_handle_write()
			{
				if (writing_state == SocketWritingStates::IDLE) return;

				// Prepare a chunk for writing

				size_t chunk_size = std::min(writing_buffer->size() - writing_buffer_offset,
					FLOW_SOCKET_WRITE_BUFFER_SIZE);
				String chunk = writing_buffer->substring(writing_buffer_offset, chunk_size);
				writing_buffer_offset += chunk_size;

				// Add the chunk to the output stream

				out_chunks.write(chunk);
			}

			static void io_write_chunk(Socket& socket, const String& chunk)
			{
				// Write the chunk

				ssize_t bytes_rw = net::write(socket.socket_fd, chunk);

				// Stop writing after buffer is fully written

				if (socket.writing_buffer_offset >= socket.writing_buffer->size()) {
					delete socket.writing_buffer;
					socket.writing_buffer_offset = 0;
					return;
				}

				// Release already written bytes

				if (socket.writing_buffer_offset >= FLOW_SOCKET_WRITE_BUFFER_RELEASE_SIZE) {
					size_t new_size = socket.writing_buffer->size() - socket.writing_buffer_offset;
					String *new_writing_buffer = new String(new_size);

					for (size_t i = 0; i < new_size; i++) {
						new_writing_buffer->set_at_index(i,
							socket.writing_buffer->get_at_index(new_size + i));
					}

					delete socket.writing_buffer;
					socket.writing_buffer = new_writing_buffer;
					socket.writing_buffer_offset = 0;
				}
			}

		public:
			int socket_fd;
			struct net::sockaddr_in client_address;

			Stream<String&> in;
			Stream<String&> out;

			Socket(int socket_fd, struct net::sockaddr_in client_address)
				: socket_fd(socket_fd), client_address(client_address),
				reading_buffer(FLOW_SOCKET_READ_BUFFER_SIZE)
			{
				net::set_nonblocking(socket_fd);

				in.start();
				out.start();

				// Todo: rethink the output model

				out_chunks.on_data([this](String& chunk) {
					io_write_chunk(*this, chunk);
				});

				out.on_data([this](String& output) {
					write(&output);
				});
			}

			void handle_io()
			{
				io_handle_read();
				io_handle_write();
			}

			void write(String *data)
			{
				if (writing_state == SocketWritingStates::WRITING) {
					writing_buffer->attach(*data);
				} else {
					writing_buffer = data;
					writing_buffer_offset = 0;
					writing_state = SocketWritingStates::WRITING;
				}
			}
	};
};

#endif