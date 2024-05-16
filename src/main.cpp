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

#include "CGI.hpp"
int main(int argc, char *argv[])
{
	CGI cgi;
	cgi.set_content_type("text/html");
	cgi.set_request_method("GET");
	cgi.set_script_name("./cgi-bin/hello.py");
	cgi.info();
	cgi.execute();
	exit(0);
	start_server(argc, argv);
	return (0);
}
