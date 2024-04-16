# include <webserver.hpp>

//all base keywords
t_vec Config::_keys = {
	"host", 
	"server_name", 
	"ports", 
	"body_size",
};

//all route keywords
t_vec Config::_keys_route = {
	"location",
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

Config::Config(t_str path)
{
	this->setup();
}

void Config::setup(void)
{
	for (t_vec::iterator it = _keys.begin(); it != _keys.end(); it++)
		_rules[*it] = "";
	for (t_vec::iterator it = _keys_route.begin(); it != _keys_route.end(); it++)
		_rules_route[*it] = "";
}

