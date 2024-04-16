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
		Ports ports;
};

#endif