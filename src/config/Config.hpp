#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <webserver.hpp>
# include "./File.hpp"

class Config
{
	private:
		class Host {
			private:
				std::string _value;
			public:
				std::string get(void) const;
				void set(std::string & value);

		};

		//needs to be done
		class ServerNames {
			private:
				std::list<std::string> _values;
			public:
				std::list<std::string> const & get(void) const;
				void set(std::list<std::string> & values);
				void show(void) const;
		};

		class Port {
			private:
				std::string _value;
			public:
				std::string get(void) const;
				void set(std::string value);
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
				Config *_parent;
			public:
				std::string _root;
				Route & get(std::string);
				void set(std::list<File::Conf> & l_routes); //pass all the information need to create a route
				void set_parent(Config *parent);
				void show(void);
		};

		class Root {
			private:
				std::string _root;
			public:
				std::string get(void) const;
				void set(std::string root);
		};

	public:
		Config(void);
		~Config(void);

		void show(void);
		
		Host host;
		Port port;
		Root root;
		Routes routes;
		BodySize body_size;
		ServerNames server_names;
};

#endif