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
		#include <netinet/in.h>
		#include <unistd.h>

		constexpr in_addr_t inaddr_any = 0;

		ssize_t read(int socket_fd, String& dest)
		{
			return read(socket_fd, dest.begin(), dest.size());
		}

		ssize_t write(int socket_fd, String& src)
		{
			return write(socket_fd, src.begin(), src.size());
		}
	};

	class Socket {
		public:
			int socket_fd;
			struct net::sockaddr_in client_address;

			enum SocketReadingStates reading_state = SocketReadingStates::READING;
			enum SocketWritingStates writing_state = SocketWritingStates::WRITING;

			String *writing_buffer;
			size_t writing_buffer_offset;

			Stream<String&> in;
			Stream<String&> out;

			Socket(int socket_fd, struct net::sockaddr_in client_address)
				: socket_fd(socket_fd), client_address(client_address) {}

			void io_handle_read()
			{
				if (reading_state == SocketReadingStates::END) return;

				// Read a chunk

				String request(FLOW_SOCKET_READ_BUFFER_SIZE);
				ssize_t bytes_rw = net::read(socket_fd, request);
				String::format("read %ld bytes from the socket\n", bytes_rw).print();

				if (bytes_rw < 0)
					String::format("read() error %ld\n", bytes_rw).print();

				// On reading end, end the reading state

				if (bytes_rw < FLOW_SOCKET_READ_BUFFER_SIZE)
					reading_state = SocketReadingStates::END;

				in.write(request);
			}

			void io_handle_write()
			{
				if (writing_state == SocketWritingStates::IDLE) return;

				// Prepare a chunk for writing

				size_t chunk_size = std::min(writing_buffer->size() - writing_buffer_offset,
					FLOW_SOCKET_WRITE_BUFFER_SIZE);
				String chunk = writing_buffer->substring(writing_buffer_offset, chunk_size);
				writing_buffer_offset += chunk_size;

				// Write the chunk

				ssize_t bytes_rw = net::write(socket_fd, chunk);

				// Stop writing after buffer is fully written

				if (writing_buffer_offset >= writing_buffer->size()) {
					delete writing_buffer;
					writing_buffer_offset = 0;
					return;
				}

				// Release already written bytes

				if (writing_buffer_offset >= FLOW_SOCKET_WRITE_BUFFER_RELEASE_SIZE) {
					size_t new_size = writing_buffer->size() - writing_buffer_offset;
					String *new_writing_buffer = new String(new_size);

					for (size_t i = 0; i < new_size; i++) {
						new_writing_buffer->set_at_index(i,
							writing_buffer->get_at_index(new_size + i));
					}

					delete writing_buffer;
					writing_buffer = new_writing_buffer;
					writing_buffer_offset = 0;
				}
			}

			void write(String *data)
			{
				if (writing_state == SocketWritingStates::WRITING) {
					writing_buffer->attach(*data);
				} else {
					writing_buffer = data;
					writing_buffer_offset = 0;
				}
			}
	};
};

#endif