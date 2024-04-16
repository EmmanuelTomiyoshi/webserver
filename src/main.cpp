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

	std::cout << std::endl;
	config.body_size.set("1024");
	std::cout << "BODY_SIZE: " << config.body_size.get() << std::endl;

	std::cout << std::endl;
	std::list<std::string> server_names;
	server_names.push_back("www.google.com"); server_names.push_back("google.com");
	config.server_names.set(server_names);
	std::cout << "SERVER_NAMES: " << config.server_names.get().front() 
		<< " " << config.server_names.get().back() << std::endl;
}

int main(int argc, char *argv[])
{
	verify_args(argc, argv);

	// Server server;
	// server.start();

	config_test();
	return (0);

}
