#include <webserver.hpp>

Parser::Parser() : _contentLength(0), _responseStatusCode(0)
{
	methods.push_back("GET");
    methods.push_back("POST");
    methods.push_back("DELETE");
}

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
		this->_requestMethod = copy._requestMethod;
		this->_requestURL = copy._requestURL;
		this->_httpVersion = copy._httpVersion;
		this->_headers = copy._headers;
		this->_contentLength = copy._contentLength;
		this->_responseStatusCode = copy._responseStatusCode;
		this->_transferEncoding = copy._transferEncoding;
		this->_messageBody = copy._messageBody;
	}
	return *this;
}



void Parser::parseRequest(const std::string &file)
{
	std::ifstream inputFile(file.c_str());

	//file validation
	if (!inputFile.is_open())
	{
		throw std::runtime_error("opening the file " + file);
	}
	else if (inputFile.seekg(0, std::ios::end)) //move to the end of the file
	{
		if (inputFile.tellg() == 0)
			throw std::runtime_error("http file is empty");
	}
	inputFile.seekg(0, std::ios::beg); //move to the beginning of the file

	std::string line;

	if (!std::getline(inputFile, line))
	{
		throw std::invalid_argument("Empty request");
	}

	std::istringstream lineStream(line);
	if (!(lineStream >> _requestMethod >> _requestURL >> _httpVersion) ||
		_requestMethod.empty() || _requestURL.empty() || _httpVersion.empty())
	{
		throw std::invalid_argument("Invalid request line");
	}

	int value = 0;
	for (StringVector::const_iterator it = methods.begin(); it != methods.end(); ++it)
	{
		if (_requestMethod == *it)
		{
			value++;
		}
	}
	if (value == 0)
	{
		throw std::invalid_argument("Invalid method: " + _requestMethod);
	}

	std::cout << COLOR_BHBLUE << _requestMethod << _requestURL << _httpVersion << COLOR_RESET << std::endl;

	// Parse headers
	// while (std::getline(inputFile, line) && !line.empty())
	// {
	// 	size_t pos = line.find(':');
	// 	if (pos != std::string::npos)
	// 	{
	// 		std::string headerName = line.substr(0, pos);
	// 		std::string headerValue = line.substr(pos + 1);
	// 		_headers[headerName] = headerValue;
	// 	}
	// }

	// // Parse Content-Length if present
	// std::map<std::string, std::string>::iterator it = _headers.find("Content-Length");
	// if (it != _headers.end())
	// {
	// 	_contentLength = std::atol(it->second.c_str());
	// }

	// // Parse Transfer-Encoding if present
	// it = _headers.find("Transfer-Encoding");
	// if (it != _headers.end())
	// {
	// 	_transferEncoding = it->second;
	// }

	// // Parse message body if present
	// char buff[2000];
	// while (!inputFile.eof())
	// {	
	// 	inputFile.getline(buff, 2000, '\0');
	// 	_messageBody += buff;
	// }
}

std::string Parser::getRequestMethod() const
{
	return _requestMethod;
}

std::string Parser::getRequestURL() const
{
	return _requestURL;
}

std::string Parser::getHTTPVersion() const
{
	return _httpVersion;
}

std::map<std::string, std::string> Parser::getHeaders() const
{
    return _headers;
}

size_t Parser::getContentLength() const
{
	return _contentLength;
}

int Parser::getResponseStatusCode() const {
    return _responseStatusCode;
}

std::string Parser::getTransferEncoding() const {
    return _transferEncoding;
}

std::string Parser::getMessageBody() const {
    return _messageBody;
}