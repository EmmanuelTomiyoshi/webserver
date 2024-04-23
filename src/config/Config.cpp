#include <webserver.hpp>

Config::Config(void)
{
}

Config::~Config(void)
{
}

void Config::show(void) const
{
	std::cout << "\n----------- SERVER CONFIGURATION -----------\n";
	this->server_names.show();
	std::cout << "Body Size: " << this->body_size.get() << std::endl;
	std::cout << "Port: " << this->port.get() << std::endl;
	std::cout << "Host: " << this->host.get() << std::endl;
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
void Config::Port::set(std::string value)
{
	this->_value = value;
}

std::string Config::Port::get(void) const
{
	return this->_value;
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

void Config::ServerNames::show(void) const
{
	std::list<std::string>::const_iterator it;
	it = this->_values.begin();
	std::cout << "ServerNames: ";
	while (it != this->_values.end())
	{
		std::cout << (*it) << " ";
		it++;
	}
	std::cout << std::endl;
}
