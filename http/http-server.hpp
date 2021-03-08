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
				OutgoingHTTPResponse&
			> request_event;

			HTTPServer()
			{
				new_socket_event.add_listener([this](Socket *socket) {
					HTTPRequestParser *parser = new HTTPRequestParser(socket->in);

					parser->headers_received_event.add_listener([parser, socket, this]() {
						IncomingHTTPRequest *req = new IncomingHTTPRequest(
							parser->body, parser->first_line, parser->headers);

						OutgoingHTTPResponse *res = new OutgoingHTTPResponse(socket->out);

						request_event.trigger(*req, *res);
					});
				});
			}
	};
};

#endif