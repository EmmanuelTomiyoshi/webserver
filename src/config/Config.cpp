#include "Config.hpp"

Config::Config(void)
{
	routes.set_parent(this);
}

Config::~Config(void)
{
}

void Config::show(void)
{
	std::cout << "\n----------- SERVER CONFIGURATION -----------\n";
	this->server_names.show();
	std::cout << "Body Size: " << this->body_size.get() << std::endl;
	std::cout << "Port: " << this->port.get() << std::endl;
	std::cout << "Host: " << this->host.get() << std::endl;
	std::cout << "Root: " << this->root.get() << std::endl;
	this->error_pages.show();
	this->routes.show();
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
void Config::Routes::set_parent(Config *parent)
{
	this->_parent = parent;
}

static void remove_last_slash(std::string & location)
{
	if (location.size() > 2 && location.at(location.size() - 1) == '/')
		location = location.substr(0, location.size() - 1);
}

Route & Config::Routes::get(std::string location)
{
	remove_last_slash(location);
	try
	{
		return this->_routes.at(location);
	}
	catch (std::exception & e)
	{
		throw std::runtime_error(HTTP_NOT_FOUND);
	}
}

void Config::Routes::set(std::list<File::Conf> & l_routes)
{	
	std::list<File::Conf>::iterator it;
	it = l_routes.begin();
	while (it != l_routes.end())
	{
		File::Conf & src = (*it);
		Route & dst = this->_routes[src._single_value["location"]];
		dst.location.set(src._single_value["location"]);
		dst.autoindex.set(src._single_value["autoindex"]);
		dst.save_files_path.set(src._single_value["save_files_path"]);
		dst.try_files.set(src._multi_values["try_files"]);
		dst.methods.set(src._multi_values["methods"]);
		dst.redirect.set(src._single_value["return"]);
		dst.set_parent_root(this->_parent->root.get());
		dst.set_root(src._single_value["root"]);
		dst.cgi_route.set(src._single_value["cgi_route"]);
		dst.cgi_extensions.set(src._multi_values["cgi_extensions"]);
		dst.error_pages.add(HTTP_BAD_REQUEST, src._single_value[HTTP_BAD_REQUEST]);
		dst.error_pages.add(HTTP_INTERNAL_SERVER_ERROR, src._single_value[HTTP_INTERNAL_SERVER_ERROR]);
		dst.error_pages.add(HTTP_METHOD_NOT_ALLOWED, src._single_value[HTTP_METHOD_NOT_ALLOWED]);
		dst.error_pages.add(HTTP_NOT_FOUND, src._single_value[HTTP_NOT_FOUND]);
		dst.error_pages.add(HTTP_OK, src._single_value[HTTP_OK]);
		dst.error_pages.add(HTTP_PAYLOAD_TOO_LARGE, src._single_value[HTTP_PAYLOAD_TOO_LARGE]);
		dst.error_pages.add(HTTP_SERVICE_UNAVAILABLE, src._single_value[HTTP_SERVICE_UNAVAILABLE]);
		it++;
	}
}

void Config::Routes::show(void)
{
	std::map<std::string, Route>::iterator it;
	it = this->_routes.begin();
	while (it != this->_routes.end())
	{
		Route & r = (*it).second;
		r.show();
		std::cout << std::endl;
		it++;
	}
}

//--------------- BodySize ------------------//
ssize_t Config::BodySize::get(void) const
{
	return this->_body_size;
}

void Config::BodySize::set(std::string value)
{
	std::stringstream ss(value);
	ss >> this->_body_size;
	if (_body_size <= 0)
		throw std::runtime_error("Config: invalid body size, expected an integer >= 1");
	if (_body_size > 150000000)
		throw std::runtime_error("Config: body size is too large, the limit is 150000000");
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

/* ----------------- ROOT ------------------ */
std::string Config::Root::get(void) const
{
	return this->_root;
}

void Config::Root::set(std::string root)
{
	this->_root = root;
}