#include "webserver.hpp"

void	verify_args(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Wrong arguments number" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string str = std::string(argv[1]);
	if (str.empty())
	{
		std::cerr << "Argument must not be empty" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[])
{
	verify_args(argc, argv);

	// Server server;
	// server.start();

	Config config;

	Methods methods;
	std::vector<std::string> allowed_methods = {"GET", "POST"};
	std::cout << std::endl;
	methods.allow(allowed_methods);
	methods.info();
	return (0);

}
