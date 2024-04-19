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

		class Root {
			private:
				std::string _root;
			public:
				std::string get(void) const;
				void set(std::string root);
		};

		class TryFiles {
			private:
				std::list<std::string> _try_files;
				std::list<std::string>::iterator _it;
			public:
				TryFiles(void);
				std::list<std::string> get(void) const;
				std::string next(void);
				void set(std::list<std::string> const & files);
		};

	public:
		Route(void);
		~Route(void);
		Route & operator=(Route const & rhs);

		Root root;
		Methods methods;
		Location location;
		TryFiles try_files;
};

#endif