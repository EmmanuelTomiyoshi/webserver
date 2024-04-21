#ifndef FILE_CPP
# define FILE_HPP
# include <webserver.hpp>

class File {
	private:
		std::fstream _file;
		std::string _data;

		std::list<std::string> _server_configs;

		void read_data(void);
		void read_config_block(void);

		static std::string read_stream(std::stringstream & ss);
		static std::string find(std::string str);

	public:
		File(std::string file_name);
		~File(void);
};

#endif