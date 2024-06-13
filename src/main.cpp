#include "webserver.hpp"
#include "Server.hpp"
#include "ft.hpp"
#include "base.hpp"

static bool is_config_file(std::string file)
{
	std::string ext = ".conf";
	file = file.substr(file.size() - ext.size());
	return file == ext;
}

void start_server(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "Wrong arguments number" << std::endl;
		exit(EXIT_FAILURE);
	}

	Server *server = NULL;
	try
	{
		if (argc == 2 && is_config_file(argv[1]))
			server = new Server(argv[1]);
		else
			server = new Server("./conf/ws.conf");
		server->start();
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	catch (std::exception & e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	if (server)
		delete server;
}

void tests(int argc, char **argv);

int main(int argc, char *argv[])
{
	try
	{
		tests(argc, argv);
		start_server(argc, argv);
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	catch(std::exception & e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	catch(...)
	{
		std::cerr << "We got a problem!!!" << std::endl;
	}
	return (0);
}

