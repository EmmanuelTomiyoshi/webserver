#ifndef PARSER_HPP
#define PARSER_HPP

#include <webserver.hpp>

typedef std::vector<std::string> StringVector;

class Parser
{
	private:
		std::string							_requestMethod;
		std::string							_requestURL;
		std::string							_httpVersion;
		std::map<std::string, std::string>	_headers;
		size_t								_contentLength;
		int									_responseStatusCode;
		std::string							_transferEncoding;
		std::string							_messageBody;

		StringVector						methods;

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