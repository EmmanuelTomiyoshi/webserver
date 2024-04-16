#ifndef ROUTE_HPP
# define ROUTE_HPP
# include <webserver.hpp>

class Methods;

class Route {
	public:
		Route(void);
		~Route(void);

		Methods methods;

		class Location {
			private:
				t_str _location;
			public:
				t_str get(void) const;
				void set(t_str value);
		};

		Location location;
};

#endif