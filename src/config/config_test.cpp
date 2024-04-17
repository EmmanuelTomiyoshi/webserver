#include <webserver.hpp>

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