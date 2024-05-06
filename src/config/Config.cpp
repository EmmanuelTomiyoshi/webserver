#include <webserver.hpp>

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

Route & Config::Routes::get(std::string location)
{
	return this->_routes.at(location);
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
		dst.set_root(this->_parent->root.get());
		it++;
	}
}

void Config::Routes::show(void)
{
	std::map<std::string, Route>::iterator it;
	it = this->_routes.begin();
	int i = 0;
	while (it != this->_routes.end())
	{
		Route & r = (*it).second;
		std::cout << "----- ROUTE "<< i++ << ":\n";
		std::cout << "Location: " << r.location.get() << std::endl;
		std::cout << "SaveFilesPath: " << r.save_files_path.get() << std::endl;
		std::cout << "Autoindex: " << (r.autoindex.get() ? "true" : "false") << std::endl;
		std::cout << "Return: " << r.redirect.get() << std::endl;
		r.methods.show();
		r.try_files.show();
		std::cout << std::endl;
		it++;
	}
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

/* ----------------- ROOT ------------------ */
std::string Config::Root::get(void) const
{
	return this->_root;
}

void Config::Root::set(std::string root)
{
	this->_root = root;
}