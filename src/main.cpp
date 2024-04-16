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

	std::vector<std::string> allowed_methods = {"GET", "POST"};
	std::cout << std::endl;
	config.route.methods.allow(allowed_methods);
	config.route.methods.info();

	std::cout << std::endl;
	config.route.location.set("/home");
	std::cout << "LOCATION: " << config.route.location.get() << std::endl;
	return (0);

}
