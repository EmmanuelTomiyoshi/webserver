#include "webserver.hpp"
#include "./parsing/Request.hpp"

std::string http_message(void) {
	return std::string(
		"GET /hello.txt HTTP/1.1\nUser-Agent: curl/7.64.1\r\nHost: www.example.com\nAccept-Language: en, mi"
	);
}

int main (void)
{
	Request req(http_message());

	std::cout << "\n--------- PRINTINGS ---------\n\n";

	std::cout << "request-line: " << req.get_request_line() << std::endl;

	std::cout << std::endl;
	req.info();
	return (0);
}