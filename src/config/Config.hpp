#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <webserver.hpp>
class Config
{
	public:
		Config(void);
		~Config(void);

		Route route;
};

#endif