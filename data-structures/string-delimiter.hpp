#ifndef FLOW_STRING_DELIMITER_HEADER
#define FLOW_STRING_DELIMITER_HEADER

#include "string.hpp"

namespace flow {
	class StringDelimiter {
		public:
			const String& str;
			size_t i = 0;

			/**
			 *  @brief  Creates a StringDelimiter instance from a given string.
			 *  @param  str  A reference to a string to delimit.
			 */
			StringDelimiter(const String& str) : str(str) {}

			/**
			 *  @brief  Gets the next token from the internal string delimited
			 *  by a given delimiter.
			 *  @param  delimiter  The character to delimit the internal string on.
			 */
			String delimit(char delimiter)
			{
				String token = str.delimit(delimiter, i);
				i += token.size() + 1;
				return token;
			}

			/**
			 *  @brief  Gets the next token from the internal string delimited
			 *  by a given delimiter.
			 *  @param  delimiter  The character sequence to delimit the internal
			 *  string on.
			 */
			template <size_t delimiter_len>
			String delimit(const char (&delimiter)[delimiter_len])
			{
				String token = str.delimit(delimiter, i);
				i += token.size() + delimiter_len - 1;
				return token;
			}

			/**
			 *  @brief  Gets the next token from the internal string delimited
			 *  by a given delimiter.
			 *  @param  delimiter  The String to delimit the internal string on.
			 */
			String delimit(const String& delimiter)
			{
				String token = str.delimit(delimiter, i);
				i += token.size() + delimiter.size();
				return token;
			}
	};
};

#endif