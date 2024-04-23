#ifndef FILE_CPP
# define FILE_HPP
# include <webserver.hpp>

class File {
	public:
		class Conf;
	private:
		std::ifstream _file;
		std::string _data;

		std::list<std::string> _server_configs;

		void fill_data(void);
		void read_config_block(void);
		void extract_blocks(void);
		void parse_blocks(void);

		static bool is_inside(std::vector<std::string> & arr, std::string & str);
		static std::string read_stream(std::stringstream & ss);
		static std::string find(std::string str);

		bool parse_single_value(std::string & block, Conf & conf);
		bool parse_multi_value(std::string & block, Conf & conf);

		std::vector<std::string> single_value_keys;
		std::vector<std::string> multi_value_keys;

	public:
		File(std::string file_name);
		~File(void);

		void info(void) const;
		std::list<Conf> confs;

		class Conf {
			public:
				std::map<std::string, std::string> _single_value;
				std::map<std::string, std::list<std::string> > _multi_values;
		};
};

#endif