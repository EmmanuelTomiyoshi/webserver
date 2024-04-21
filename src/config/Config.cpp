#include <webserver.hpp>

Config::Config(std::string file_name)
{
	std::fstream file(file_name);
	if (file.bad())
		throw std::runtime_error("failed to open '" + file_name + "'");
}

Config::~Config(void)
{
}

//--------------- Host ------------------//
void Config::Host::set(std::string & value)
{
	this->_value = value;
}

std::string Config::Host::get(void) const
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

//--------------- Routes ------------------//
Route & Config::Routes::get(std::string location)
{
	return this->_routes.at(location);
}

void Config::Routes::set(std::string location)
{
	Route & route = this->_routes[location];
	route.location.set(location);
	route.methods.allow("GET");
}


//--------------- BodySize ------------------//
int Config::BodySize::get(void) const
{
	return this->_body_size;
}

void Config::BodySize::set(std::string value)
{
	std::stringstream ss(value);
	ss >> this->_body_size;
}

//--------------- ServerName ------------------//
std::list<std::string> const & Config::ServerNames::get(void) const
{
	return this->_values;
}

void Config::ServerNames::set(std::list<std::string> & values)
{
	std::list<std::string>::iterator it;
	it = values.begin();
	while (it != values.end())
	{
		this->_values.push_back(*it);
		it++;
	}
}
