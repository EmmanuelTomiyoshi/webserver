#include "webserver.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Wrong arguments number" << std::endl;
		return (EXIT_FAILURE);
	}

	std::string str = std::string(argv[1]);
	if (str.empty())
	{
		std::cerr << "Argument must not be empty" << std::endl;
		return (EXIT_FAILURE);
	}

	IPResolver ip;

	std::string hostname = "google.com";
	IPResolver resolver;
	resolver.printIPAdresses(hostname);

	Server server;
	server.start();
	return (0);

}
