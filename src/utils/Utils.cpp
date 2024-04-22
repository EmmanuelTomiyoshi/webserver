#include <webserver.hpp>

namespace utils
{
	Utils::Utils()
	{
        HttpStatusCode[100] = "Continue";
        HttpStatusCode[101] = "Switching Protocols";
        HttpStatusCode[102] = "Processing";
        HttpStatusCode[103] = "Early Hints";
        HttpStatusCode[200] = "OK";
        HttpStatusCode[201] = "Created";
        HttpStatusCode[202] = "Accepted";
        HttpStatusCode[203] = "Non-Authoritative Information";
        HttpStatusCode[204] = "No Content";
        HttpStatusCode[205] = "Reset Content";
        HttpStatusCode[206] = "Partial Content";
        HttpStatusCode[207] = "Multi-Status";
        HttpStatusCode[208] = "Already Reported";
        HttpStatusCode[226] = "IM Used";
        HttpStatusCode[300] = "Multiple Choices";
        HttpStatusCode[301] = "Moved Permanently";
        HttpStatusCode[302] = "Found";
        HttpStatusCode[303] = "See Other";
        HttpStatusCode[304] = "Not Modified";
        HttpStatusCode[305] = "Use Proxy";
        HttpStatusCode[307] = "Temporary Redirect";
        HttpStatusCode[308] = "Permanent Redirect";
        HttpStatusCode[400] = "Bad Request";
        HttpStatusCode[401] = "Unauthorized";
        HttpStatusCode[402] = "Payment Required";
        HttpStatusCode[403] = "Forbidden";
        HttpStatusCode[404] = "Not Found";
        HttpStatusCode[405] = "Method Not Allowed";
        HttpStatusCode[406] = "Not Acceptable";
        HttpStatusCode[407] = "Proxy Authentication Required";
        HttpStatusCode[408] = "Request Timeout";
        HttpStatusCode[409] = "Conflict";
        HttpStatusCode[410] = "Gone";
        HttpStatusCode[411] = "Length Required";
        HttpStatusCode[412] = "Precondition Failed";
        HttpStatusCode[413] = "Payload Too Large";
        HttpStatusCode[414] = "URI Too Long";
        HttpStatusCode[415] = "Unsupported Media Type";
        HttpStatusCode[416] = "Range Not Satisfiable";
        HttpStatusCode[417] = "Expectation Failed";
        HttpStatusCode[418] = "I'm a teapot";
        HttpStatusCode[421] = "Misdirected Request";
        HttpStatusCode[422] = "Unprocessable Entity";
        HttpStatusCode[423] = "Locked";
        HttpStatusCode[424] = "Failed Dependency";
        HttpStatusCode[425] = "Too Early";
        HttpStatusCode[426] = "Upgrade Required";
        HttpStatusCode[428] = "Precondition Required";
        HttpStatusCode[429] = "Too Many Requests";
        HttpStatusCode[431] = "Request Header Fields Too Large";
        HttpStatusCode[451] = "Unavailable For Legal Reasons";
        HttpStatusCode[500] = "Internal Server Error";
        HttpStatusCode[501] = "Not Implemented";
        HttpStatusCode[502] = "Bad Gateway";
        HttpStatusCode[503] = "Service Unavailable";
        HttpStatusCode[504] = "Gateway Timeout";
        HttpStatusCode[505] = "HTTP Version Not Supported";
        HttpStatusCode[506] = "Variant Also Negotiates";
        HttpStatusCode[507] = "Insufficient Storage";
        HttpStatusCode[508] = "Loop Detected";
        HttpStatusCode[510] = "Not Extended";
        HttpStatusCode[511] = "Network Authentication Required";
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
}
