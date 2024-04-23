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
		this->blocks.push_back(config);
	}
}

Configs::~Configs(void)
{
}

