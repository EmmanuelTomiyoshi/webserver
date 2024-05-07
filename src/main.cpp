#include "webserver.hpp"

void start_server(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "Wrong arguments number" << std::endl;
		exit(EXIT_FAILURE);
	}

	Server *server;
	try
	{
		if (argc == 2)
			server = new Server(argv[1]);
		else
			server = new Server("./conf/ws.conf");
		server->start();
	}
	catch (std::exception & e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	delete server;
}

int main(int argc, char *argv[])
{
	start_server(argc, argv);
	return (0);
}
