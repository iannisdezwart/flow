#ifndef FLOW_HTTP_RESPONSE_HEADER
#define FLOW_HTTP_RESPONSE_HEADER

#include "../data-structures/string.hpp"
#include "../data-structures/string-delimiter.hpp"
#include "../data-structures/stream.hpp"
#include "../networking/socket.hpp"

namespace flow_http_tools {
	using namespace flow;

	constexpr const char *HTTP_VERSION = "HTTP/1.1";

	enum class HTTPMethods {
		UNDEF = -1,
		GET,
		HEAD,
		POST,
		PUT,
		DELETE,
		CONNECT,
		OPTIONS,
		TRACE,
		PATCH
	};

	const char *to_string(enum HTTPMethods method)
	{
		switch (method) {
			default:
			case HTTPMethods::GET: return "GET";
			case HTTPMethods::HEAD: return "HEAD";
			case HTTPMethods::POST: return "POST";
			case HTTPMethods::PUT: return "PUT";
			case HTTPMethods::DELETE: return "DELETE";
			case HTTPMethods::CONNECT: return "CONNECT";
			case HTTPMethods::OPTIONS: return "OPTIONS";
			case HTTPMethods::TRACE: return "TRACE";
			case HTTPMethods::PATCH: return "PATCH";
		}
	}

	enum HTTPMethods str_to_method(const String& str)
	{
		if (str == "GET") return HTTPMethods::GET;
		if (str == "HEAD") return HTTPMethods::HEAD;
		if (str == "POST") return HTTPMethods::POST;
		if (str == "PUT") return HTTPMethods::PUT;
		if (str == "DELETE") return HTTPMethods::DELETE;
		if (str == "CONNECT") return HTTPMethods::CONNECT;
		if (str == "OPTIONS") return HTTPMethods::OPTIONS;
		if (str == "TRACE") return HTTPMethods::TRACE;
		if (str == "PATCH") return HTTPMethods::PATCH;
		return HTTPMethods::UNDEF;
	}

	enum class HTTPStatusCodes {
		CONTINUE = 100,
		SWITCHING_PROTOCOL = 101,
		PROCESSING = 102,
		EARLY_HINTS = 103,

		OK = 200,
		CREATED = 201,
		ACCEPTED = 202,
		NON_AUTHORITATIVE_INFORMATION = 203,
		NO_CONTENT = 204,
		RESET_CONTENT = 205,
		PARTIAL_CONTENT = 206,
		MULTI_STATUS = 207,
		ALREADY_REPORTED = 208,
		IM_USED = 226,

		MULTIPLE_CHOICE = 300,
		MOVED_PERMANENTLY = 301,
		FOUND = 302,
		SEE_OTHER = 303,
		NOT_MODIFIED = 304,
		TEMPORARY_REDIRECT = 307,
		PERMANENT_REDIRECT = 308,

		BAD_REQUEST = 400,
		UNAUTHORIZED = 401,
		PAYMENT_REQUIRED = 402,
		FORBIDDEN = 403,
		NOT_FOUND = 404,
		METHOD_NOT_ALLOWED = 405,
		NOT_ACCEPTABLE = 406,
		PROXY_AUTHENTICATION_REQUIRED = 407,
		REQUEST_TIMEOUT = 408,
		CONFLICT = 409,
		GONE = 410,
		LENGTH_REQUIRED = 411,
		PRECONDITION_FAILED = 412,
		PAYLOAD_TOO_LARGE = 413,
		URI_TOO_LONG = 414,
		UNSUPPORTED_MEDIA_TYPE = 415,
		RANGE_NOT_SATISFIABLE = 416,
		EXPECTATION_FAILED = 417,
		IM_A_TEAPOT = 418,
		MISDIRECTED_REQUEST = 421,
		UNPROCESSABLE_ENTITY = 422,
		LOCKED = 423,
		FAILED_DEPENDENCY = 424,
		TOO_EARLY = 425,
		UPGRADE_REQUIRED = 426,
		PRECONDITION_REQUIRED = 428,
		TOO_MANY_REQUESTS = 429,
		REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
		UNAVAILABLE_FOR_LEGAL_REASONS = 451,

		INTERNAL_SERVER_ERROR = 500,
		NOT_IMPLEMENTED = 501,
		BAD_GATEWAY = 502,
		SERVICE_UNAVAILABLE = 503,
		GATEWAY_TIMEOUT = 504,
		HTTP_VERSION_NOT_SUPPORTED = 505,
		VARIANT_ALSO_NEGOTIATES = 506,
		INSUFFICIENT_STORAGE = 507,
		LOOP_DETECTED = 508,
		NOT_EXTENDED = 510,
		NETWORK_AUTHENTICATION_REQUIRED = 511
	};

	const char *to_string(enum HTTPStatusCodes status_code)
	{
		switch (status_code) {
			case HTTPStatusCodes::CONTINUE: return "100 Continue";
			case HTTPStatusCodes::SWITCHING_PROTOCOL: return "101 Switching Protocol";
			case HTTPStatusCodes::PROCESSING: return "102 Processing";
			case HTTPStatusCodes::EARLY_HINTS: return "103 Early Hints";

			default:
			case HTTPStatusCodes::OK: return "200 OK";
			case HTTPStatusCodes::CREATED: return "201 Created";
			case HTTPStatusCodes::ACCEPTED: return "202 Accepted";
			case HTTPStatusCodes::NON_AUTHORITATIVE_INFORMATION: return "203 Non-Authoritative Information";
			case HTTPStatusCodes::NO_CONTENT: return "204 No Content";
			case HTTPStatusCodes::RESET_CONTENT: return "205 Reset Content";
			case HTTPStatusCodes::PARTIAL_CONTENT: return "206 Partial Content";
			case HTTPStatusCodes::MULTI_STATUS: return "207 Multi-Status";
			case HTTPStatusCodes::ALREADY_REPORTED: return "208 Already Reported";
			case HTTPStatusCodes::IM_USED: return "226 IM Used";

			case HTTPStatusCodes::MULTIPLE_CHOICE: return "300 Multiple Choice";
			case HTTPStatusCodes::MOVED_PERMANENTLY: return "301 Moved Permanently";
			case HTTPStatusCodes::FOUND: return "302 Found";
			case HTTPStatusCodes::SEE_OTHER: return "303 See Other";
			case HTTPStatusCodes::NOT_MODIFIED: return "304 Not Modified";
			case HTTPStatusCodes::TEMPORARY_REDIRECT: return "307 Temporary Redirect";
			case HTTPStatusCodes::PERMANENT_REDIRECT: return "308 Permanent Redirect";

			case HTTPStatusCodes::BAD_REQUEST: return "400 Bad Request";
			case HTTPStatusCodes::UNAUTHORIZED: return "401 Unauthorized";
			case HTTPStatusCodes::PAYMENT_REQUIRED: return "402 Payment Required";
			case HTTPStatusCodes::FORBIDDEN: return "403 Forbidden";
			case HTTPStatusCodes::NOT_FOUND: return "404 Not Found";
			case HTTPStatusCodes::METHOD_NOT_ALLOWED: return "405 Method Not Allowed";
			case HTTPStatusCodes::NOT_ACCEPTABLE: return "406 Not Acceptable";
			case HTTPStatusCodes::PROXY_AUTHENTICATION_REQUIRED: return "407: Proxy Authentication Required";
			case HTTPStatusCodes::REQUEST_TIMEOUT: return "408 Request Timeout";
			case HTTPStatusCodes::CONFLICT: return "409 Conflict";
			case HTTPStatusCodes::GONE: return "410 Gone";
			case HTTPStatusCodes::LENGTH_REQUIRED: return "411 Length Required";
			case HTTPStatusCodes::PRECONDITION_FAILED: return "412 Precondition Failed";
			case HTTPStatusCodes::PAYLOAD_TOO_LARGE: return "413 Payload Too Large";
			case HTTPStatusCodes::URI_TOO_LONG: return "414 URI Too Long";
			case HTTPStatusCodes::UNSUPPORTED_MEDIA_TYPE: return "415 Unsupported Media Type";
			case HTTPStatusCodes::RANGE_NOT_SATISFIABLE: return "416 Range Not Satisfiable";
			case HTTPStatusCodes::EXPECTATION_FAILED: return "417 Expectation Failed";
			case HTTPStatusCodes::IM_A_TEAPOT: return "418 I'm a teapot"; // Not capitalised in standard
			case HTTPStatusCodes::MISDIRECTED_REQUEST: return "421 Misdirected Request";
			case HTTPStatusCodes::UNPROCESSABLE_ENTITY: return "422 Unprocessable Entity";
			case HTTPStatusCodes::LOCKED: return "423 Locked";
			case HTTPStatusCodes::FAILED_DEPENDENCY: return "424 Failed Dependency";
			case HTTPStatusCodes::TOO_EARLY: return "425 Too Early";
			case HTTPStatusCodes::UPGRADE_REQUIRED: return "426 Upgrade Required";
			case HTTPStatusCodes::PRECONDITION_REQUIRED: return "428 Precondition Required";
			case HTTPStatusCodes::TOO_MANY_REQUESTS: return "429 Too Many Requests";
			case HTTPStatusCodes::REQUEST_HEADER_FIELDS_TOO_LARGE: return "431 Request Header Fields Too Large";
			case HTTPStatusCodes::UNAVAILABLE_FOR_LEGAL_REASONS: return "451 Unavailable For Legal Reasons";

			case HTTPStatusCodes::INTERNAL_SERVER_ERROR: return "500 Internal Server Error";
			case HTTPStatusCodes::NOT_IMPLEMENTED: return "501 Not Implemented";
			case HTTPStatusCodes::BAD_GATEWAY: return "502 Bad Gateway";
			case HTTPStatusCodes::SERVICE_UNAVAILABLE: return "503 Service Unavailable";
			case HTTPStatusCodes::GATEWAY_TIMEOUT: return "504 Gateway Timeout";
			case HTTPStatusCodes::HTTP_VERSION_NOT_SUPPORTED: return "505 HTTP Version Not Supported";
			case HTTPStatusCodes::VARIANT_ALSO_NEGOTIATES: return "506 Variant Also Negotiates";
			case HTTPStatusCodes::INSUFFICIENT_STORAGE: return "507 Insufficient Storage";
			case HTTPStatusCodes::LOOP_DETECTED: return "508 Loop Detected";
			case HTTPStatusCodes::NOT_EXTENDED: return "509 Not Extended";
			case HTTPStatusCodes::NETWORK_AUTHENTICATION_REQUIRED: return "511 Network Authentication Required";
		}
	}

	class IncomingHTTPMessage {
		public:
			const std::unordered_map<String, String>& headers;
			Stream<String&>& body;

			IncomingHTTPMessage(
				Stream<String&>& body_input,
				const std::unordered_map<String, String>& headers
			) : body(body_input), headers(headers) {}

			template <size_t key_len>
			const String& get_header(const char (&key)[key_len]) const
			{
				String key_str = key;
				return get_header(key);
			}

			const String& get_header(const String& key) const
			{
				return headers.at(key);
			}
	};

	class OutgoingHTTPMessage {
		public:
			std::unordered_map<String, String> headers;
			Stream<String&>& body;

			OutgoingHTTPMessage(
				Stream<String&>& socket_output
			) : body(socket_output) {}

			template <size_t key_len>
			const String& get_header(const char (&key)[key_len]) const
			{
				String key_str = key;
				return get_header(key);
			}

			const String& get_header(const String& key) const
			{
				return headers.at(key);
			}

			template <size_t key_len, size_t value_len>
			void set_header(const char (&key)[key_len], const char (&value)[value_len])
			{
				String key_str = key;
				String value_str = value;

				set_header(key_str, value_str);
			}

			void set_header(const String& key, const String& value)
			{
				headers[key] = value;
			}

			String build_headers()
			{
				// Calculate size

				size_t size = 0;

				for (std::pair<String, String> header : headers) {
					const String& key = header.first;
					const String& value = header.second;

					size += key.size() + value.size() + 3;
				}

				size += 1;

				// Build headers string

				String headers_str(size);

				for (std::pair<String, String> header : headers) {
					const String& key = header.first;
					const String& value = header.second;

					headers_str += String::format("%S: %S\n", key, value);
				}

				headers_str += '\n';

				return headers_str;
			}
	};

	struct HTTPRequestFirstLine {
		enum HTTPMethods method;
		String path;
		String http_version;

		String build()
		{
			return String::format("%s %S %S\n", to_string(method), path, http_version);
		}
	};

	struct HTTPResponseFirstLine {
		String http_version;
		enum HTTPStatusCodes status_code;

		String build()
		{
			return String::format("%S %s\n", http_version, to_string(status_code));
		}
	};

	class IncomingHTTPRequest : public IncomingHTTPMessage {
		public:
			const HTTPRequestFirstLine& first_line;

			IncomingHTTPRequest(
				Stream<String&>& body_input,
				const HTTPRequestFirstLine& first_line,
				const std::unordered_map<String, String>& headers
			) : IncomingHTTPMessage(body_input, headers), first_line(first_line) {}
	};

	class IncomingHTTPResponse : public IncomingHTTPMessage {
		public:
			const HTTPResponseFirstLine& first_line;

			IncomingHTTPResponse(
				Stream<String&>& body_input,
				const HTTPResponseFirstLine& first_line,
				const std::unordered_map<String, String>& headers
			) : IncomingHTTPMessage(body_input, headers), first_line(first_line) {}
	};

	class OutgoingHTTPRequest : public OutgoingHTTPMessage {
		public:
			HTTPRequestFirstLine first_line;

			OutgoingHTTPRequest(
				Stream<String&>& socket_output
			) : OutgoingHTTPMessage(socket_output)
			{
				first_line.http_version = "HTTP/1.1";
			}

			void send(enum HTTPMethods method, const String& path)
			{
				first_line.method = method;
				first_line.path = path;

				// Queue the first line for writing

				String first_line_str = first_line.build();
				body.write(first_line_str);

				// Queue the headers for writing

				String headers_str = build_headers();
				body.write(headers_str);
			}
	};

	class OutgoingHTTPResponse : public OutgoingHTTPMessage {
		public:
			HTTPResponseFirstLine first_line;

			OutgoingHTTPResponse(
				Stream<String&>& socket_output
			) : OutgoingHTTPMessage(socket_output)
			{
				first_line.http_version = "HTTP/1.1";
			}

			void send(enum HTTPStatusCodes status_code)
			{
				first_line.status_code = status_code;

				// Queue the first line for writing

				String first_line_str = first_line.build();
				body.write(first_line_str);

				// Queue the headers for writing

				String headers_str = build_headers();
				body.write(headers_str);
			}
	};

	enum class HTTPRequestParserStates {
		PARSING_FIRST_LINE,
		PARSING_HEADERS,
		PARSING_BODY,
		FINISHED_PARSING
	};

	enum class HTTPRequestParserErrors {
		UNKNOWN_METHOD,
		MALFORMED_HEADER,
	};

	class HTTPRequestParser {
		public:
			String buffer;

			enum HTTPRequestParserStates state =
				HTTPRequestParserStates::PARSING_FIRST_LINE;

			// Required to create the IncomingHTTPRequest

			HTTPRequestFirstLine first_line;
			std::unordered_map<String, String> headers;
			Stream<String&> body;

			// Events

			EventEmitter<> first_line_received_event;
			EventEmitter<> headers_received_event;

			HTTPRequestParser(Stream<String&>& stream)
				: buffer(String(FLOW_SOCKET_READ_BUFFER_SIZE))
			{
				stream.on_data([&](String& chunk) {
					// Convert CRLF to LF

					chunk.replace("\r\n", "\n");

					if (
						state == HTTPRequestParserStates::PARSING_FIRST_LINE ||
						state == HTTPRequestParserStates::PARSING_HEADERS
					) buffer += chunk;

					switch (state) {
						case HTTPRequestParserStates::PARSING_FIRST_LINE:
						{
							ssize_t newline_index = buffer.first_index_of('\n');

							// If the newline was not found, wait for it
							// Todo: cap max buffer size

							if (newline_index == -1) return;

							StringDelimiter delimiter(buffer);

							// Read method

							String method_str = delimiter.delimit(' ');
							first_line.method = str_to_method(method_str);

							if (first_line.method == HTTPMethods::UNDEF)
								throw HTTPRequestParserErrors::UNKNOWN_METHOD;

							// Read path

							first_line.path = delimiter.delimit(' ');

							// Expect HTTP protocol version

							first_line.http_version = delimiter.delimit('\n');

							// Todo: check HTTP protocol version

							first_line_received_event.trigger();

							// Clear buffer

							buffer = buffer.substring(delimiter.offset);

							// Advance to the next state and fall through

							state = HTTPRequestParserStates::PARSING_HEADERS;
						}

						parse_next_header:
						case HTTPRequestParserStates::PARSING_HEADERS:
						{
							ssize_t newline_index = buffer.first_index_of('\n');

							// If a newline was not found, wait for it

							if (newline_index == -1) return;

							StringDelimiter delimiter(buffer);

							// Get the header line

							String header_line = delimiter.delimit('\n');

							if (header_line.size() > 0 && header_line != "\n") {
								// Parse the header line

								ssize_t colon_index = header_line.first_index_of(':');

								if (colon_index == -1) throw HTTPRequestParserErrors::MALFORMED_HEADER;

								// Seperate the key and value

								String header_key = header_line.between(0, colon_index - 1);
								String header_value = header_line.substring(colon_index + 2);

								headers[header_key] = header_value;

								// Clear buffer

								buffer = buffer.substring(delimiter.offset);
								goto parse_next_header;
							} else {
								headers_received_event.trigger();

								// Start the body Stream
								// Todo: extract newlines and clear buffer

								body.start();
								body.write(buffer);

								// Advance to the next state and fall through

								state = HTTPRequestParserStates::PARSING_BODY;
							}
						}

						case HTTPRequestParserStates::PARSING_BODY:
						{
							body.write(chunk);

							// Todo: stop the body Stream when the last chunk arrives
						}
					}
				});
			}
	};
};

namespace flow {
	using namespace flow_http_tools;
};

#endif