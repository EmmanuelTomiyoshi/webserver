#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <webserver.hpp>
class Config
{
	private:
		class Host {
			private:
				std::string _host;
			public:
				std::string get(void) const;
				void set(std::string value);
		};

	public:
		Config(void);
		~Config(void);

		Route route;
		Host host;
};

#endif