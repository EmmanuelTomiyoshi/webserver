#ifndef PARSER_HPP
#define PARSER_HPP

#include <webserver.hpp>

class Parser
{
	private:
		std::string	requestMethod;
		std::string	requestURL;
		std::string	httpVersion;
		std::map<std::string, std::string> headers;
		size_t	contentLength;
		int		responseStatusCode;
		std::string	transferEncoding;
		std::string messageBody;

	public:
		Parser();
		Parser(const Parser &copy);
		Parser &operator=(const Parser &copy);
		~Parser();

		// Method to parse HTTP request
		void parseRequest(const std::string &requestString);

		// Methods to get request components
		std::string getRequestMethod() const;
		std::string getRequestURL() const;
		std::string getHTTPVersion() const;
		std::map<std::string, std::string> getHeaders() const;
		size_t getContentLength() const;
		int getResponseStatusCode() const;
		std::string getTransferEncoding() const;
		std::string getMessageBody() const;
};

#endif //PARSER_HPP