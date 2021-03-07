#ifndef FLOW_SOCKET_SERVER_HEADER
#define FLOW_SOCKET_SERVER_HEADER

#include "../data-structures/dynamic-array.hpp"
#include "../data-structures/string.hpp"
#include "../networking/socket.hpp"

namespace flow {
	class SocketServer {
		public:
			int socket_fd;
			uint16_t port;
			struct net::sockaddr_in server_address;

			DynamicArray<Socket *> client_sockets;

			EventEmitter<Socket *> new_socket_event;

			SocketServer()
			{
				socket_fd = net::socket(AF_INET, net::SOCK_STREAM, 0);
				net::set_nonblocking(socket_fd);
				if (socket_fd < 0) throw "Error opening socket";
			}

			void listen_to(uint16_t port)
			{
				server_address.sin_family = AF_INET;
				server_address.sin_addr.s_addr = net::inaddr_any;
				#if __BYTE_ORDER == __BIG_ENDIAN
				server_address.sin_port = port;
				#else
				server_address.sin_port = __bswap_16(port);
				#endif

				if (net::bind(socket_fd, (struct net::sockaddr *) &server_address,
					sizeof(server_address)) < 0) throw "Error binding socket";

				net::listen(socket_fd, 5); // Todo: figure out what the 5 means

				int client_socket_fd;

				struct net::sockaddr_in client_address;
				net::socklen_t client_address_length = sizeof(client_address);

				while (true) {
					client_socket_fd = net::accept(socket_fd,
						(struct net::sockaddr *) &client_address, &client_address_length);

					if (client_socket_fd >= 0) {
						// Create new socket

						Socket* socket = new Socket(client_socket_fd, client_address);
						client_sockets.append(socket);

						new_socket_event.trigger(socket);
					} else if (errno != EWOULDBLOCK) {
						String::format("accept() error %d, errno = %d\n",
							client_socket_fd, errno).print();
					}

					// Handle IO on sockets

					for (size_t i = 0; i < client_sockets.size(); i++) {
						client_sockets[i]->handle_io();
					}
				}
			}
	};
};

#endif