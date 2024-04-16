#include <webserver.hpp>

Config::Config(void)
{
	std::cout << "Config created!\n";
}

Config::~Config(void)
{
}

//--------------- Host ------------------//
void Config::Host::set(std::string value)
{
	this->_value = value;
}

std::string Config::Host::get(void) const
{
	return this->_value;
}

//--------------- ServerName ------------------//
void Config::ServerName::set(std::string value)
{
	this->_value = value;
}

std::string Config::ServerName::get(void) const
{
	return this->_value;
}

//--------------- Ports ------------------//
void Config::Ports::set(std::list<std::string> values)
{
	std::list<std::string>::iterator it;
	it = values.begin();
	while (it != values.end())
	{
		this->_values.push_back(*it);
		it++;
	}
}

std::list<std::string> Config::Ports::get(void) const
{
	return this->_values;
}

void Config::Ports::info(void) const
{
	std::list<std::string>::const_iterator it;
	it = this->_values.begin();
	std::cout << "PORTS: ";
	while (it != this->_values.end())
	{
		std::cout << *it << " ";
		it++;
	}
	std::cout << std::endl;
}
