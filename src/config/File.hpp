#ifndef FILE_CPP
# define FILE_HPP
# include <webserver.hpp>

class File {
	private:
		std::fstream _file;
		std::string _data;

		std::list<std::string> _server_configs;

		void fill_data(void);
		void read_config_block(void);
		void extract_blocks(void);
		void parse_blocks(void);
		void parse_single_value(std::string & block);
		void parse_multi_value(std::string & block);

		static std::string read_stream(std::stringstream & ss);
		static std::string find(std::string str);

		std::map<std::string, std::string> _single_value;
		std::map<std::string, std::list<std::string>> _multi_values;

	public:
		File(std::string file_name);
		~File(void);
};

#endif