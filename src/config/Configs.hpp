#ifndef CONFIGS_HPP
# define CONFIGS_HPP
# include <webserver.hpp>

class Configs {
	private:
		int _len;
		File _file;

	public:
		Configs(std::string config_file);
		~Configs(void);

		int len(void) const;
		std::vector<Config> blocks;
};

#endif