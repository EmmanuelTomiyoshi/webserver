#include <webserver.hpp>

Config::Config(void)
{
	std::cout << "Config created!\n";
}

Config::~Config(void)
{
}

void Config::Host::set(std::string value)
{
	this->_host = value;
}

std::string Config::Host::get(void) const
{
	return this->_host;
}
