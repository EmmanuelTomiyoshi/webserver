#include "webserver.hpp"
#include "./parsing/Request.hpp"

int main (void)
{
	Request req("eu sou a lei");

	std::cout << req.get_message() << std::endl;
	return (0);
}