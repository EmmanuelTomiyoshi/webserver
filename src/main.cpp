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

	Config config("sellatos");

	std::cout << "\nSTRATEGY 2" << std::endl;
	// Server server;
	// server.start();

	
	return (0);

}
