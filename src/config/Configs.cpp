#include <webserver.hpp>

Configs::Configs(std::string config_file) : _file(config_file)
{
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
		this->_configs.push_back(config);
	}
	this->_it = this->_configs.begin();
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


Configs::~Configs(void)
{
}

