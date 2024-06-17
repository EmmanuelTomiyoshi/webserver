#ifndef ROUTE_HPP
# define ROUTE_HPP
# include "base.hpp"
# include "Methods.hpp"
# include "ErrorPages.hpp"

class Route
{
	private:
		std::string _parent_root;
		std::string _root;

		class Location {
			private:
				std::string _location;
			public:
				std::string get(void) const;
				void set(std::string value);
		};

		class TryFiles {
			private:
				std::list<std::string> _try_files;
				std::list<std::string>::iterator _it;
			public:
				TryFiles(void);
				std::list<std::string> const & get(void) const;
				std::string next(void);
				void set(std::list<std::string> const & files);
				void show(void);
		};

		class SaveFilesPath {
			private:
				std::string _root;
				std::string _save_files_path;
			public:
				void set_root(std::string root);
				std::string get(void) const;
				void set(std::string path);
		};

		class Autoindex {
			private:
				bool _on;
			public:
				Autoindex(void);
				bool get(void) const;
				void set(bool value);
				void set(std::string value);
		};

		class CGI_Route {
			private:
				bool _value;
			public:
				CGI_Route(void);
				bool is_true(void) const;
				void set(bool value);
				void set(std::string value);
		};

		class Return {
			private:
				std::string _value;
			public:
				std::string get(void) const;
				void set(std::string value);
		};

	public:
		Route(void);
		~Route(void);
		Route & operator=(Route const & rhs);

		Methods methods;
		Return redirect;
		Location location;
		TryFiles try_files;
		Autoindex autoindex;
		CGI_Route cgi_route;
		SaveFilesPath save_files_path;
		ErrorPages error_pages;

		std::string get_page(void);
		void set_root(std::string root);
		void set_parent_root(std::string root);
		std::string get_root(void) const;
		std::string get_path(void) const;
		void show(void);
		void validate(void);
};

#endif