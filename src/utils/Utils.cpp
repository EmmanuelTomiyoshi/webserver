#include <webserver.hpp>

namespace utils
{
	//orthodoxical cannonical form
	Utils::Utils()
	{
        _HttpStatusCode[100] = "Continue";
        _HttpStatusCode[101] = "Switching Protocols";
        _HttpStatusCode[102] = "Processing";
        _HttpStatusCode[103] = "Early Hints";
        _HttpStatusCode[200] = "OK";
        _HttpStatusCode[201] = "Created";
        _HttpStatusCode[202] = "Accepted";
        _HttpStatusCode[203] = "Non-Authoritative Information";
        _HttpStatusCode[204] = "No Content";
        _HttpStatusCode[205] = "Reset Content";
        _HttpStatusCode[206] = "Partial Content";
        _HttpStatusCode[207] = "Multi-Status";
        _HttpStatusCode[208] = "Already Reported";
        _HttpStatusCode[226] = "IM Used";
        _HttpStatusCode[300] = "Multiple Choices";
        _HttpStatusCode[301] = "Moved Permanently";
        _HttpStatusCode[302] = "Found";
        _HttpStatusCode[303] = "See Other";
        _HttpStatusCode[304] = "Not Modified";
        _HttpStatusCode[305] = "Use Proxy";
        _HttpStatusCode[307] = "Temporary Redirect";
        _HttpStatusCode[308] = "Permanent Redirect";
        _HttpStatusCode[400] = "Bad Request";
        _HttpStatusCode[401] = "Unauthorized";
        _HttpStatusCode[402] = "Payment Required";
        _HttpStatusCode[403] = "Forbidden";
        _HttpStatusCode[404] = "Not Found";
        _HttpStatusCode[405] = "Method Not Allowed";
        _HttpStatusCode[406] = "Not Acceptable";
        _HttpStatusCode[407] = "Proxy Authentication Required";
        _HttpStatusCode[408] = "Request Timeout";
        _HttpStatusCode[409] = "Conflict";
        _HttpStatusCode[410] = "Gone";
        _HttpStatusCode[411] = "Length Required";
        _HttpStatusCode[412] = "Precondition Failed";
        _HttpStatusCode[413] = "Payload Too Large";
        _HttpStatusCode[414] = "URI Too Long";
        _HttpStatusCode[415] = "Unsupported Media Type";
        _HttpStatusCode[416] = "Range Not Satisfiable";
        _HttpStatusCode[417] = "Expectation Failed";
        _HttpStatusCode[418] = "I'm a teapot";
        _HttpStatusCode[421] = "Misdirected Request";
        _HttpStatusCode[422] = "Unprocessable Entity";
        _HttpStatusCode[423] = "Locked";
        _HttpStatusCode[424] = "Failed Dependency";
        _HttpStatusCode[425] = "Too Early";
        _HttpStatusCode[426] = "Upgrade Required";
        _HttpStatusCode[428] = "Precondition Required";
        _HttpStatusCode[429] = "Too Many Requests";
        _HttpStatusCode[431] = "Request Header Fields Too Large";
        _HttpStatusCode[451] = "Unavailable For Legal Reasons";
        _HttpStatusCode[500] = "Internal Server Error";
        _HttpStatusCode[501] = "Not Implemented";
        _HttpStatusCode[502] = "Bad Gateway";
        _HttpStatusCode[503] = "Service Unavailable";
        _HttpStatusCode[504] = "Gateway Timeout";
        _HttpStatusCode[505] = "HTTP Version Not Supported";
        _HttpStatusCode[506] = "Variant Also Negotiates";
        _HttpStatusCode[507] = "Insufficient Storage";
        _HttpStatusCode[508] = "Loop Detected";
        _HttpStatusCode[510] = "Not Extended";
        _HttpStatusCode[511] = "Network Authentication Required";
	}

	Utils::Utils(const Utils &copy)
	{
		*this = copy;
	}


	Utils &Utils::operator=(const Utils &copy)
	{
		if (this != &copy)
		{

		}
		return *this;
	}

	Utils::~Utils() { }

	//getters
	std::map<int, std::string> Utils::getHttpStatusCode() const
	{
		return _HttpStatusCode;
	}

	//util functions
	bool Utils::hasInvalidURICharacters(const std::string &str)
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
}
