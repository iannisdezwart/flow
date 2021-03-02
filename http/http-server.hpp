#ifndef FLOW_HTTP_SERVER_HEADER
#define FLOW_HTTP_SERVER_HEADER

#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../data-structures/string.hpp"
#include "http-message.hpp"

using namespace std;

namespace flow {
	class HTTPServer {
		private:
			String read_request(int client_socket_fd)
			{
				String request;
				char buf[4096];
				ssize_t bytes_rw;

				while (true) {
					bytes_rw = read(client_socket_fd, buf, 4095);
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
			int client_socket_fd;
			uint16_t port;
			struct sockaddr_in server_address;
			struct sockaddr_in client_address;

			HTTPServer()
			{
				socket_fd = socket(AF_INET, SOCK_STREAM, 0);
				if (socket_fd < 0) throw "Error opening socket";
			}

			void listen_to(uint16_t port)
			{
				server_address.sin_family = AF_INET;
				server_address.sin_addr.s_addr = INADDR_ANY;
				server_address.sin_port = htons(port);

				if (bind(socket_fd, (struct sockaddr *) &server_address,
					sizeof(server_address)) < 0) throw "Error binding socket";

				listen(socket_fd, 5); // Todo: figure out what the 5 means

				socklen_t client_address_length = sizeof(client_address);
				char buf[4096];

				while (true) {
					client_socket_fd = accept(socket_fd,
						(struct sockaddr *) &client_address, &client_address_length);

					if (client_socket_fd < 0)
						printf("accept() error %d\n", client_socket_fd);

					printf("Got new connection: %d\n", client_socket_fd);

					String req_str = read_request(client_socket_fd);
					HTTPRequestMessage req = HTTPRequestMessage::parse(req_str);
				}
			}
	};
};

#endif