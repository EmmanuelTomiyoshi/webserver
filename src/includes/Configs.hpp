#ifndef CONFIGS_HPP
# define CONFIGS_HPP
# include "base.hpp"
# include "Config.hpp"

class Configs {
	private:
		std::list<Config>::iterator _it;
		File _file;
		std::list<Config> _configs;

	public:
		Configs(std::string config_file);
		~Configs(void);

		void init(void);

		Config & next(void);
		int len(void) const;
		void reset_iterator(void);
		std::list<Config> & get(void);
		void show(void);
		
		std::map<int, Config*> _fdconfigs;
};

#endif