#include "webserver.hpp"
#include "./parsing/Request.hpp"

int main (void)
{
	Request req;

	req.hello();
	webtest();
	return (0);
}