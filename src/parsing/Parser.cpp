#include "Parser.hpp"

Parser::Parser() : contentLength(0), responseStatusCode(0) { }

Parser::Parser(const Parser &copy)
{
	*this = copy;
}

Parser::~Parser() { }

Parser &Parser::operator=(const Parser &copy)
{
	if (this != &copy)
	{
		// Copy the request/response components
		this->requestMethod = copy.requestMethod;
		this->requestURL = copy.requestURL;
		this->httpVersion = copy.httpVersion;
		this->headers = copy.headers;
		this->contentLength = copy.contentLength;
		this->responseStatusCode = copy.responseStatusCode;
		this->transferEncoding = copy.transferEncoding;
		this->messageBody = copy.messageBody;
	}
	return *this;
}

void Parser::parseRequest(const std::string &requestString)
{
	std::istringstream iss(requestString);
	std::string line;

	// Parse request line
	if (!std::getline(iss, line))
	{
		throw std::invalid_argument("Empty request");
	}

	std::istringstream lineStream(line);
	if (!(lineStream >> requestMethod >> requestURL >> httpVersion) ||
		requestMethod.empty() || requestURL.empty() || httpVersion.empty())
	{
		throw std::invalid_argument("Invalid request line");
	}

	// Parse headers
	while (std::getline(iss, line) && !line.empty())
	{
		size_t pos = line.find(':');
		if (pos != std::string::npos)
		{
			std::string headerName = line.substr(0, pos);
			std::string headerValue = line.substr(pos + 1);
			headers[headerName] = headerValue;
		}
	}

	// Parse Content-Length if present
	std::map<std::string, std::string>::iterator it = headers.find("Content-Length");
	if (it != headers.end())
	{
		contentLength = std::atol(it->second.c_str());
	}

	// Parse Transfer-Encoding if present
	it = headers.find("Transfer-Encoding");
	if (it != headers.end())
	{
		transferEncoding = it->second;
	}

	// Parse message body if present
	std::stringstream messageBodyStream;
	messageBodyStream >> messageBody;
}

std::string Parser::getRequestMethod() const
{
	return requestMethod;
}

std::string Parser::getRequestURL() const
{
	return requestURL;
}

std::string Parser::getHTTPVersion() const
{
	return httpVersion;
}

std::map<std::string, std::string> Parser::getHeaders() const
{
    return headers;
}

size_t Parser::getContentLength() const
{
	return contentLength;
}

int Parser::getResponseStatusCode() const {
    return responseStatusCode;
}

std::string Parser::getTransferEncoding() const {
    return transferEncoding;
}

std::string Parser::getMessageBody() const {
    return messageBody;
}