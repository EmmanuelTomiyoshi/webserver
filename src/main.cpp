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

void config_test(void)
{
	Config config;

	std::cout << std::endl;
	config.routes.set("/janaina");
	config.routes.set("/home");

	std::cout << std::endl;
	std::cout << "JANAINA INFO:\n";
	config.routes.get("/janaina").methods.info();
	std::cout << "LOCATION: " << config.routes.get("/janaina").location.get() << std::endl;

	std::cout << std::endl;
	config.host.set("localhost");
	std::cout << "HOST: " << config.host.get() << std::endl;

	std::cout << std::endl;
	std::list<std::string> ports;
	ports.push_back("3050"); ports.push_back("3002");
	config.ports.set(ports);
	config.ports.info();
}

int main(int argc, char *argv[])
{
	verify_args(argc, argv);

	// Server server;
	// server.start();

	config_test();
	return (0);

}
