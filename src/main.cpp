#include "webserver.hpp"
#include "Server.hpp"
#include "ft.hpp"

void cgi(void)
{
	int pid = fork();

	if (pid == 0)
	{
		char python[] = "/bin/python3";
		char script[] = "./cgi-bin/hello.py";
		char *argv[] = {python, script, NULL};
		execv(python, argv);
	}
	
}

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

	Server *server;
	try
	{
		if (argc == 2 && is_config_file(argv[1]))
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

void temp(void);
void tests(int argc, char **argv);

int main(int argc, char *argv[])
{
	tests(argc, argv);
	start_server(argc, argv);
	return (0);
}

