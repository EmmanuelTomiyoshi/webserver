#ifndef CONFIGS_HPP
# define CONFIGS_HPP
# include <webserver.hpp>

class Configs {
	private:
		std::list<Config>::iterator _it;
		File _file;
		std::list<Config> _configs;

	public:
		Configs(std::string config_file);
		~Configs(void);

		Config & next(void);
		int len(void) const;
		void reset_iterator(void);
};

#endif