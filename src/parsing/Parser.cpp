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
		this->_messageBody = copy._messageBody;
	}
	return *this;
}


bool hasInvalidURICharacters(const std::string &str)
{
	std::string invalidChars = " \t\n\r\f\a\b\e\v";
	std::string traversalSequence = "../"; //for security reasons

	if (str.find_first_of(invalidChars) != std::string::npos ||
		str.find(traversalSequence) != std::string::npos)
	{
		return true;
	}
	return false;
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


	//gets string for the first line
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

	//validation for methods
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

	//URI validation
	std::string invalidChars = " \t\n\r\f$|<>";
	if (_requestURL.at(0) != '/' || hasInvalidURICharacters(_requestURL) || _requestURL.size() > MAX_URI_LENGTH)
	{
		throw std::invalid_argument("Invalid URI: " + _requestURL);
	}

	//HTTP Version validation

	if (_httpVersion != WEBSERVER_HTTP_VERSION)
	{
		throw std::invalid_argument("Invalid HTTP Version: " + _httpVersion);
	}

	std::cout << COLOR_BHBLUE << _requestMethod << _requestURL << _httpVersion << COLOR_RESET << std::endl;

	// Parse headers
	while (std::getline(inputFile, line) && !line.empty())
	{
		size_t pos = line.find(':');
		if (pos != std::string::npos)
		{
			std::string headerName = line.substr(0, pos);
			std::string headerValue = line.substr(pos + 1);
			_headers[headerName] = headerValue;
		}
	}

	// Parse Content-Length
	std::map<std::string, std::string>::iterator it = _headers.find("Content-Length");
	if (it != _headers.end())
	{
		_contentLength = std::atol(it->second.c_str());
	}

	// Parse message body if present
	char buff[MESSAGE_BODY_BUFFER];
	while (!inputFile.eof())
	{	
		inputFile.getline(buff, MESSAGE_BODY_BUFFER, '\0');
		_messageBody += buff;
	}

	std::cout << _contentLength << " " << _messageBody.size() << std::endl;
	if (_contentLength <= 0 || _messageBody.size() > _contentLength)
	{
		throw std::invalid_argument("Content length differs from the size of the message body");
	}

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

std::string Parser::getMessageBody() const {
    return _messageBody;
}