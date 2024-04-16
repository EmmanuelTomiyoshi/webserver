#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <webserver.hpp>

//create easy ways of retrieving values

typedef struct ROUTE {
	t_str location;
	t_map methods;
	t_str root;
	t_map try_files;
	t_map cgi_extensions;
	t_str save_files_path;
	bool autoindex;
} ROUTE;

typedef struct RULES {
	t_str host;
	t_str server_name;
	t_map ports;
	t_str body_size;
	std::map<t_str, ROUTE> routes;
} RULES;

class Config
{
	private:
		static t_vec _keys;
		static t_vec _keys_route;
		t_map _rules;
		t_map _rules_route;

		void setup(void);


	public:
		Config(void);
		Config(t_str path);
		Config(Config const & rhs);
		~Config(void);

		class Route {
			private:
				class Location {
					private:
						t_str _location;
					public:
						t_str get(void) const;
						void set(t_str value);
				};
			public:
				Location location;
		};



		Route route;
};

#endif