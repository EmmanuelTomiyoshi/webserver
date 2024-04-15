#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <webserver.hpp>
# include <map>
# include <vector>
# include <algorithm>

typedef std::string t_str;
typedef std::map<t_str, t_str> t_map;
typedef std::vector<t_str> t_vec;

class Config
{
	private:
		static t_vec _keys;
		static t_vec _route_keys;
		t_map _rules;
		t_map _rules_route;

		void setup(void);
		void set_rule(t_str key, t_str value);
	public:
		Config(void);
		Config(t_str path);
		Config(Config const & rhs);
		~Config(void);

		t_str get_rule(t_str key) const;
};

#endif