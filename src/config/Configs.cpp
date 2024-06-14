#include "Configs.hpp"

Configs::Configs(std::string config_file) : _file(config_file)
{
}

void Configs::init(void)
{
	_file.init();
	std::list<File::Conf>::iterator it;
	it = _file.confs.begin();
	for (; it != _file.confs.end(); it++)
	{
		File::Conf & conf = (*it);
		Config config;
		config.server_names.set(conf._multi_values["server_name"]);
		config.body_size.set(conf._single_value["body_size"]);
		config.host.set(conf._single_value["host"]);
		config.port.set(conf._single_value["port"]);
		config.root.set(conf._single_value["root"]);
		config.error_pages.add(HTTP_BAD_REQUEST, conf._single_value[HTTP_BAD_REQUEST]);
		config.error_pages.add(HTTP_INTERNAL_SERVER_ERROR, conf._single_value[HTTP_INTERNAL_SERVER_ERROR]);
		config.error_pages.add(HTTP_METHOD_NOT_ALLOWED, conf._single_value[HTTP_METHOD_NOT_ALLOWED]);
		config.error_pages.add(HTTP_NOT_FOUND, conf._single_value[HTTP_NOT_FOUND]);
		config.error_pages.add(HTTP_OK, conf._single_value[HTTP_OK]);
		config.error_pages.add(HTTP_PAYLOAD_TOO_LARGE, conf._single_value[HTTP_PAYLOAD_TOO_LARGE]);
		config.error_pages.add(HTTP_SERVICE_UNAVAILABLE, conf._single_value[HTTP_SERVICE_UNAVAILABLE]);
		config.routes.set(conf._routes);
		this->_configs.push_back(config);
		_ports.push_back(conf._single_value["port"]);
	}
	this->_it = this->_configs.begin();
	validate_ports();
	setup_server_names();
}

void Configs::validate_ports(void)
{
	std::sort(_ports.begin(), _ports.end());
	std::vector<std::string>::iterator it;
	it = std::adjacent_find(_ports.begin(), _ports.end());
	if (it != _ports.end())
		throw std::runtime_error("duplicated ports found in config file");
}

void Configs::setup_server_names(void)
{
	std::string content = ft::read_file("/etc/hosts");
	std::string loopback = "127.0.0.1";

	std::stringstream buff;

	std::list<Config>::iterator it;
	it = _configs.begin();
	for (; it != _configs.end(); it++)
	{
		if (it->server_names.get().size() <= 1)
			continue ;
		std::list<std::string>::const_iterator it2;
		it2 = it->server_names.get().begin();
		for (; it2 != --(it->server_names.get().end()); it2++)
		{
			std::string host = loopback + " " + (*it2) + "\n";
			if (content.find(host) != std::string::npos)
			{
				std::cout << "already exists: " << host;
				continue ;
			}
			buff << host;
		}
	}
	std::string host = loopback + " " + "localhost\n";
	if (content.find(host) == std::string::npos)
		buff << host;
	std::string result = buff.str() + content;
	ft::debug_file("/etc/hosts", result.c_str(), result.size());
}

Config & Configs::next(void)
{
	if (_it == _configs.end())
		throw new std::runtime_error("end of configs");
	Config & config = (*_it);
	_it++;
	return config;
}

void Configs::reset_iterator(void)
{
	_it = _configs.begin();
}

int Configs::len(void) const
{
	return _configs.size();
}

void Configs::show(void)
{
	std::list<Config>::iterator it;
	it = this->_configs.begin();
	int i = 0;
	while (it != this->_configs.end())
	{
		std::cout << "----------------- SERVER " << i++ << " -----------------\n";
		(*it).show();
		std::cout << std::endl;
		it++;
	}
}

std::list<Config> & Configs::get(void)
{
	return this->_configs;
}


Configs::~Configs(void)
{
}

