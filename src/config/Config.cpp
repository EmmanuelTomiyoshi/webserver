# include <webserver.hpp>

Config::Config(void)
{}

Config::~Config(void)
{}

Config::Config(Config const & rhs)
{}

Config::Config(std::string path)
{
	std::cout << "Opening: " << path << std::endl;
}
