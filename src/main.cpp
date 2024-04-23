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

	Server server;
	server.start();
	config_test();

	try {

		Parser parser;
		parser.parseRequest("y_http.http");

		std::cout << "Request Method: " << parser.getRequestMethod() << std::endl;
		std::cout << "Request URL: " << parser.getRequestURL() << std::endl;
		std::cout << "HTTP Version: " << parser.getHTTPVersion() << std::endl;
		std::map<std::string, std::string> headers = parser.getHeaders();
		std::cout << "Headers:\n";

        for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		{
            std::cout << it->first << ": " << it->second << std::endl;
        }
	
		std::cout << "Content-Length: " << parser.getContentLength() << std::endl;
		std::cout << "Message Body:\n" << parser.getMessageBody() << std::endl;

		}
		catch (const std::exception& ex)
		{
			std::cerr << "Error: " << ex.what() << std::endl;
			return 1;
		}
	return (0);
}
