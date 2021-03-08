#ifndef FLOW_SOCKET_HEADER
#define FLOW_SOCKET_HEADER

#include "../data-structures/stream.hpp"
#include "../data-structures/string.hpp"
#include "../data-structures/queue.hpp"

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

			Queue<String *> write_queue;

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

				in.write(reading_buffer);
			}

			void io_handle_write()
			{
				if (writing_state == SocketWritingStates::IDLE) return;
				if (write_queue.size() == 0) {
					writing_state = SocketWritingStates::IDLE;
					return;
				}

				// Write the next chunk

				String *chunk = write_queue.pop();
				ssize_t bytes_rw = net::write(socket_fd, *chunk);
				delete chunk;
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

				out.on_data([this](String& data) {
					// Split String into chunks and add the chunks to the write queue

					String *chunk;
					size_t i = 0;
					size_t data_size = data.size();

					while (i < data_size) {
						size_t chunk_size = std::min(data_size,
							FLOW_SOCKET_WRITE_BUFFER_SIZE);

						chunk = new String(chunk_size);
						chunk->unsafe_increment_element_count(chunk_size);

						for (size_t j = 0; j < chunk_size; j++) {
							chunk->set_at_index(j, data[i + j]);
						}

						write_queue.push(chunk);
						i += chunk_size;
					}

					// Enable writing state

					writing_state = SocketWritingStates::WRITING;
				});
			}

			void handle_io()
			{
				io_handle_read();
				io_handle_write();
			}
	};
};

#endif