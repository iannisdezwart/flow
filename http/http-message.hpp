#ifndef FLOW_HTTP_RESPONSE_HEADER
#define FLOW_HTTP_RESPONSE_HEADER

#include <bits/stdc++.h>

#include "../data-structures/string.hpp"

using namespace std;

namespace flow_http_tools {
	using namespace flow;

	constexpr const char *HTTP_VERSION = "HTTP/1.1";

	enum HTTPMethods {
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
			case GET: return "GET";
			case HEAD: return "HEAD";
			case POST: return "POST";
			case PUT: return "PUT";
			case DELETE: return "DELETE";
			case CONNECT: return "CONNECT";
			case OPTIONS: return "OPTIONS";
			case TRACE: return "TRACE";
			case PATCH: return "PATCH";
		}
	}

	enum HTTPMethods str_to_method(const String& str)
	{
		if (str == "GET") return GET;
		if (str == "HEAD") return HEAD;
		if (str == "POST") return POST;
		if (str == "PUT") return PUT;
		if (str == "DELETE") return DELETE;
		if (str == "CONNECT") return CONNECT;
		if (str == "OPTIONS") return OPTIONS;
		if (str == "TRACE") return TRACE;
		if (str == "PATCH") return PATCH;
		return UNDEF;
	}

	enum HTTPStatusCodes {
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
			case CONTINUE: return "100 Continue";
			case SWITCHING_PROTOCOL: return "101 Switching Protocol";
			case PROCESSING: return "102 Processing";
			case EARLY_HINTS: return "103 Early Hints";

			default:
			case OK: return "200 OK";
			case CREATED: return "201 Created";
			case ACCEPTED: return "202 Accepted";
			case NON_AUTHORITATIVE_INFORMATION: return "203 Non-Authoritative Information";
			case NO_CONTENT: return "204 No Content";
			case RESET_CONTENT: return "205 Reset Content";
			case PARTIAL_CONTENT: return "206 Partial Content";
			case MULTI_STATUS: return "207 Multi-Status";
			case ALREADY_REPORTED: return "208 Already Reported";
			case IM_USED: return "226 IM Used";

			case MULTIPLE_CHOICE: return "300 Multiple Choice";
			case MOVED_PERMANENTLY: return "301 Moved Permanently";
			case FOUND: return "302 Found";
			case SEE_OTHER: return "303 See Other";
			case NOT_MODIFIED: return "304 Not Modified";
			case TEMPORARY_REDIRECT: return "307 Temporary Redirect";
			case PERMANENT_REDIRECT: return "308 Permanent Redirect";

			case BAD_REQUEST: return "400 Bad Request";
			case UNAUTHORIZED: return "401 Unauthorized";
			case PAYMENT_REQUIRED: return "402 Payment Required";
			case FORBIDDEN: return "403 Forbidden";
			case NOT_FOUND: return "404 Not Found";
			case METHOD_NOT_ALLOWED: return "405 Method Not Allowed";
			case NOT_ACCEPTABLE: return "406 Not Acceptable";
			case PROXY_AUTHENTICATION_REQUIRED: return "407: Proxy Authentication Required";
			case REQUEST_TIMEOUT: return "408 Request Timeout";
			case CONFLICT: return "409 Conflict";
			case GONE: return "410 Gone";
			case LENGTH_REQUIRED: return "411 Length Required";
			case PRECONDITION_FAILED: return "412 Precondition Failed";
			case PAYLOAD_TOO_LARGE: return "413 Payload Too Large";
			case URI_TOO_LONG: return "414 URI Too Long";
			case UNSUPPORTED_MEDIA_TYPE: return "415 Unsupported Media Type";
			case RANGE_NOT_SATISFIABLE: return "416 Range Not Satisfiable";
			case EXPECTATION_FAILED: return "417 Expectation Failed";
			case IM_A_TEAPOT: return "418 I'm a teapot"; // Not capitalised in standard
			case MISDIRECTED_REQUEST: return "421 Misdirected Request";
			case UNPROCESSABLE_ENTITY: return "422 Unprocessable Entity";
			case LOCKED: return "423 Locked";
			case FAILED_DEPENDENCY: return "424 Failed Dependency";
			case TOO_EARLY: return "425 Too Early";
			case UPGRADE_REQUIRED: return "426 Upgrade Required";
			case PRECONDITION_REQUIRED: return "428 Precondition Required";
			case TOO_MANY_REQUESTS: return "429 Too Many Requests";
			case REQUEST_HEADER_FIELDS_TOO_LARGE: return "431 Request Header Fields Too Large";
			case UNAVAILABLE_FOR_LEGAL_REASONS: return "451 Unavailable For Legal Reasons";

			case INTERNAL_SERVER_ERROR: return "500 Internal Server Error";
			case NOT_IMPLEMENTED: return "501 Not Implemented";
			case BAD_GATEWAY: return "502 Bad Gateway";
			case SERVICE_UNAVAILABLE: return "503 Service Unavailable";
			case GATEWAY_TIMEOUT: return "504 Gateway Timeout";
			case HTTP_VERSION_NOT_SUPPORTED: return "505 HTTP Version Not Supported";
			case VARIANT_ALSO_NEGOTIATES: return "506 Variant Also Negotiates";
			case INSUFFICIENT_STORAGE: return "507 Insufficient Storage";
			case LOOP_DETECTED: return "508 Loop Detected";
			case NOT_EXTENDED: return "509 Not Extended";
			case NETWORK_AUTHENTICATION_REQUIRED: return "511 Network Authentication Required";
		}
	}

	class HTTPMessage {
		public:
			String start_line;
			unordered_map<String, String> headers;
			String body;

			HTTPMessage(const String& start_line)
			{
				this->start_line = start_line;
			}

			template <size_t key_len, size_t value_len>
			void set_header(const char (&key)[key_len], const char (&value)[value_len])
			{
				String key_str = key;
				String value_str = value;

				set_header(key_str, value_str);
			}

			void set_header(String& key, String& value)
			{
				headers[key] = value;
			}

			template <size_t key_len>
			String& get_header(const char (&key)[key_len])
			{
				String key_str = key;

				return get_header(key);
			}

			String& get_header(String& key)
			{
				return headers[key];
			}

			String build()
			{
				size_t size = 0;

				for (pair<const String&, const String&> entry : headers) {
					const String& key = entry.first;
					const String& value = entry.second;

					size += key.size() + value.size() + 3; // ':' + ' ' + '\n'
				}

				String headers_str(size);

				for (pair<const String&, const String&> entry : headers) {
					const String& key = entry.first;
					const String& value = entry.second;

					headers_str += String::format("%S: %S\n", key, value);
				}

				return String::format("%S\n%S\n", start_line, headers_str);
			}
	};

	class HTTPRequestMessage : public HTTPMessage {
		public:
			enum HTTPMethods method;
			String path;

			HTTPRequestMessage(enum HTTPMethods method, const String& path)
				: method(method), path(path), HTTPMessage(
					String::format("%s %S %s", to_string(method), path, HTTP_VERSION)
				) {}

			template <size_t path_len>
			HTTPRequestMessage(enum HTTPMethods method, const char (&path)[path_len])
				: method(method), path(path), HTTPMessage(
					String::format("%s %S %s", to_string(method), String(path), HTTP_VERSION)
				) {}

			static HTTPRequestMessage parse(String& req_str)
			{
				// Convert CRLF to LF

				req_str.replace("\r\n", "\n");
				req_str.print();
				StringDelimiter delimiter(req_str);

				// Read method

				String method_str = delimiter.delimit(' ');
				enum HTTPMethods method = str_to_method(method_str);

				// Read path

				String path = delimiter.delimit(' ');

				// Expect HTTP protocol version

				String http_version = delimiter.delimit('\n');

				HTTPRequestMessage req(method, path);

				// Read headers

				while (true) {
					String header_line = delimiter.delimit('\n');
					if (header_line.size() == 0 || header_line == "\n") break;

					size_t colon_index = header_line.first_index_of(':');
					String header_key = header_line.between(0, colon_index - 1);
					String header_value = header_line.substring(colon_index + 2);

					String::format("<Header (%lld, %lld)> %S -> %S",
						header_key.size(), header_value.size(), header_key, header_value).print();

					req.set_header(header_key, header_value);
				}

				String::format("method = %d, path = %S, ver = %S", method, path, http_version).print();
				return req;
			}
	};

	class HTTPResponseMessage : public HTTPMessage {
		public:
			enum HTTPMethods method;

			HTTPResponseMessage(enum HTTPStatusCodes status_code)
				: method(method), HTTPMessage(
					String::format("%s %s", HTTP_VERSION, to_string(status_code))
				) {}
	};
};

namespace flow {
	using namespace flow_http_tools;
};

#endif