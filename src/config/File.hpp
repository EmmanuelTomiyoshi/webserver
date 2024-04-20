#ifndef FILE_CPP
# define FILE_HPP
# include <webserver.hpp>

class File {
	private:
		std::string _data;

		std::list<std::string> _server_configs;

		void read_config_block(void);

	public:
		File(std::string file_name);
		~File(void);
};

#endif