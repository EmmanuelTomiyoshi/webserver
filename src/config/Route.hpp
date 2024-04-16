#ifndef ROUTE_HPP
# define ROUTE_HPP
# include <webserver.hpp>

class Route
{
	private:
		class Location {
			private:
				std::string _location;
			public:
				std::string get(void) const;
				void set(std::string value);
		};

	public:
		Route(void);
		~Route(void);

		Methods methods;

		Location location;
};

#endif