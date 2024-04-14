# include <webserver.hpp>

t_vec Config::_keys = {
	"host", 
	"server_name", 
	"ports", 
	"body_size",
	"location",

};

t_vec Config::_route_keys = {
	"methods",
	"root",
	"try_files",
	"cgi_extensions",
	"save_files_path",
	"autoindex",
	"return"
};

Config::Config(void)
{}

Config::~Config(void)
{}

Config::Config(Config const & rhs)
{}

Config::Config(std::string path)
{
	std::cout << "Opening: " << path << std::endl;

	std::cout << "KEYS:\n";
	for (t_vec::iterator it = _keys.begin(); it != _keys.end(); it++)
		std::cout << *it << std::endl;

	std::cout << "\nROUTE_KEYS:\n";
	for (t_vec::iterator it = _keys.begin(); it != _keys.end(); it++)
		std::cout << *it << std::endl;
}

