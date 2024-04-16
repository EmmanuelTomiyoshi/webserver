#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <webserver.hpp>
class Config
{
	private:
		class Host {
			private:
				std::string _value;
			public:
				std::string get(void) const;
				void set(std::string value);
		};

		class ServerName {
			private:
				std::string _value;
			public:
				std::string get(void) const;
				void set(std::string value);
		};

		class Ports {
			private:
				std::list<std::string> _values;
			public:
				std::list<std::string> get(void) const;
				void set(std::list<std::string> values);
				void info(void) const;
		};

		class BodySize {
			private:
				int _body_size;
			public:
				int get(void) const;
				void set(std::string value);
		};

		class Routes {
			private:
				std::map<std::string, Route> _routes;
			public:
				Route const & get(std::string) const;
				void set(std::string info); //pass all the information need to create a route
		};

	public:
		Config(void);
		~Config(void);

		
		Host host;
		Ports ports;
		Routes routes;
		BodySize body_size;
};

#endif