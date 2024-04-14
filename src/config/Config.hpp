#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <webserver.hpp>
# include <map>
# include <vector>

typedef std::map<std::string, std::string> t_map;
typedef std::vector<std::string> t_vec;

class Config
{
	private:
		static t_vec _keys;
		static t_vec _route_keys;
		t_map _rules;
		t_map _rules_route;

		void setup(void);
	public:
		Config(void);
		Config(std::string path);
		Config(Config const & rhs);
		~Config(void);

};

#endif