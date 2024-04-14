#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <webserver.hpp>

class Config
{
	public:
		Config(void);
		Config(std::string path);
		Config(Config const & rhs);
		~Config(void);

};

#endif