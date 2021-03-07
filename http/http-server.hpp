#ifndef FLOW_HTTP_SERVER_HEADER
#define FLOW_HTTP_SERVER_HEADER

#include "../data-structures/string.hpp"
#include "http-message.hpp"
#include "../networking/socket.hpp"
#include "../networking/socket-server.hpp"
#include "http-message.hpp"

namespace flow {
	using namespace flow_http_tools;

	class HTTPServer : public SocketServer {
		public:
			EventEmitter<
				const IncomingHTTPRequest&,
				const OutgoingHTTPResponse&
			> request_event;

			HTTPServer()
			{
				new_socket_event.add_listener([this](Socket *socket) {
					String::format("HTTPServer got a new socket: %d", socket->socket_fd).print();
					HTTPRequestParser *parser = new HTTPRequestParser(socket->in);

					parser->headers_received_event.add_listener([parser, socket, this]() {
						IncomingHTTPRequest *req = new IncomingHTTPRequest(
							parser->body, parser->first_line, parser->headers);

						OutgoingHTTPResponse *res = new OutgoingHTTPResponse(socket->out);

						request_event.trigger(*req, *res);
					});

					// socket->in.on_data([socket](String& data) {
					// 	String::format("Received %lld bytes from socket %d: %S",
					// 		data.size(), socket->socket_fd, data).print();
					// });
				});
			}
	};

	class _HTTPServer {
		private:
			String read_request(int client_socket_fd)
			{
				String request;
				char buf[4096];
				ssize_t bytes_rw;

				while (true) {
					bytes_rw = net::read(client_socket_fd, buf, 4095);
					printf("read %ld bytes from the socket\n", bytes_rw);

					if (bytes_rw < 0)
						printf("read() error %ld\n", bytes_rw);

					request.attach_c_str(buf);

					if (bytes_rw < 4095) break;
				}

				// String::format("[ Request, length = %ld ]\n%S\n[ End Request ]",
				// 	bytes_rw, request).print();

				// HTTPResponseMessage res(OK);
				// res.set_header("Server", "Flow-Server");
				// res.set_header("Content-Type", "text/plain");
				// res.set_header("Content-Length", "13");
				// String res_str = res.build() + "Hello, World!";

				// bytes_rw = write(client_socket_fd, res_str.begin(), res_str.size());

				// if (bytes_rw < 0)
				// 	printf("write() error %ld\n", bytes_rw);

				// printf("Wrote a %ld bytes response\n", bytes_rw);

				return request;
			}

		public:
			int socket_fd;
			uint16_t port;
			struct net::sockaddr_in server_address;

			DynamicArray<Socket *> client_sockets;

			_HTTPServer()
			{
				socket_fd = net::socket(AF_INET, net::SOCK_STREAM, 0);
				net::set_nonblocking(socket_fd);
				if (socket_fd < 0) throw "Error opening socket";
			}

			void listen_to(uint16_t port)
			{
				server_address.sin_family = AF_INET;
				server_address.sin_addr.s_addr = net::inaddr_any;
				server_address.sin_port = net::htons(port);

				if (net::bind(socket_fd, (struct net::sockaddr *) &server_address,
					sizeof(server_address)) < 0) throw "Error binding socket";

				net::listen(socket_fd, 5); // Todo: figure out what the 5 means

				int client_socket_fd;

				struct net::sockaddr_in client_address;
				net::socklen_t client_address_length = sizeof(client_address);

				while (true) {
					net::set_nonblocking(client_socket_fd);

					client_socket_fd = net::accept(socket_fd,
						(struct net::sockaddr *) &client_address, &client_address_length);

					if (client_socket_fd >= 0) {
						// Create new socket

						printf("new socket: %d\n", client_socket_fd);

						Socket* socket = new Socket(client_socket_fd, client_address);
						client_sockets.append(socket);

						// String req_str = read_request(client_socket_fd);
						// HTTPRequestMessage req = HTTPRequestMessage::parse(req_str);
					} else if (errno != EWOULDBLOCK) {
						printf("accept() error %d, errno = %d\n", client_socket_fd, errno);
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