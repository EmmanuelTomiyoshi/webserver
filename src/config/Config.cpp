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

Config::Config(t_str path)
{
	std::cout << "Opening: " << path << std::endl;

	std::cout << "KEYS:\n";


	std::cout << "\nROUTE_KEYS:\n";
	for (t_vec::iterator it = _keys.begin(); it != _keys.end(); it++)
		std::cout << *it << std::endl;

	this->setup();
	this->set_rule("host", "localhost");
}

void Config::setup(void)
{
	for (t_vec::iterator it = _keys.begin(); it != _keys.end(); it++)
		_rules[*it] = "";

	std::cout << "rule: " << _rules["server_name"] << std::endl;
}

void Config::set_rule(const t_str key, t_str value)
{	
	try
	{
		t_str & rule = _rules.at(key);
		rule = value;
	}
	catch(const std::exception& e)
	{
		std::cerr << "INVALID RULE: " << key << '\n';
	}
}

t_str Config::get_rule(t_str key) const
{
	try
	{
		return this->_rules.at(key);
	}
	catch (std::exception & e)
	{
		return t_str("");
	}
}
