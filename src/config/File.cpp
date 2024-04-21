#include <webserver.hpp>

/* 
	_server_configs: uma string dessa lista tem o bloco de código 
					 com todas as configurações pra rodar um server

	1- ler o bloco de codigo todo dentro de uma string e armazenar ela dentro dessa lista
	2- repetir até ter as configurações de todos os servidores
	3- iterar na lista criando uma Config a partir de cada string _server_configs
	4- criar uma classe que roda um servidor com os dados da classe Config
	5- criar todos os servidores que precisar

 */

File::File(std::string file_name) : _file(file_name)
{
	if (this->_file.bad())
		throw std::runtime_error("failed to open '" + file_name + "'");
	read_data();
	while (_data.empty() == false)
	{
		try
		{
			read_config_block();
		}
		catch (std::exception & e)
		{
			std::cerr << e.what() << std::endl;
			break ;
		}
	}
}

void File::read_data(void)
{
	char buffer[2000];
	while (!_file.eof())
	{
		_file.getline(buffer, 2000, '\0');
		_data += buffer;
	}
}

std::string File::read_stream(std::stringstream & ss)
{
	std::string str;
	char buffer[2000];
	while (!ss.eof())
	{
		ss.getline(buffer, 2000, '\0');
		str += buffer;
	}
	return str;
}

/* 

	size_t index = str.find("server");
	if (index == std::string::npos)
		throw std::runtime_error("server not found");
	if (str.find_first_not_of("server") < index)
		throw std::runtime_error("server not found");
	str = str.substr(index + std::string("server").length());

	std::cout << str << std::endl;
	//find the open brackets
	open = str.find('{');
	if (index == std::string::npos)
		throw std::runtime_error("'{' not found");
	if (str.find_first_not_of('{') < open)
		throw std::runtime_error("'{' not found");
	str = str.substr(open + 1);
 */

void File::read_config_block(void)
{
	std::stringstream ss(_data);
	std::string word;
	size_t open;
	size_t close;

	ss >> word;
	if (word.empty())
	{
		_data.erase(0);
		return ;
	}
	if (word != "server")
		throw std::runtime_error("found '" + word + "' instead of 'server'");
	ss >> word;
	if (word != "{")
		throw std::runtime_error("'{' not found");
	open = _data.find('{');

	//jump all the subblocks inside brackets
	size_t inside = open + 1;
	while (true)
	{
		size_t inside_open = _data.find('{', inside);
		if (inside_open != std::string::npos)
		{
			size_t inside_close = _data.find('}', inside);
			if (inside_close == std::string::npos)
				throw std::runtime_error("inside close brackets not found");
			if (inside_close < inside_open)
				break ;
			inside = inside_close + 1;
			continue ;
		}
		break ;
	}

	//get server close bracket index
	close = _data.find('}', inside);
	if (close == std::string::npos)
		throw std::runtime_error("server close bracket not found");

	/* the substr func do not allow to pass an end index 
	 so the option is to use the size
	 so to get to the end index using the size we need to subtract the open
	 and to not get the } bracket we need to subtract one more byte*/
	std::string block = _data.substr(open + 1, close - open - 1);

	_data = _data.substr(close + 1);
	_server_configs.push_back(block);
	std::cout << "------------------ BLOCK ------------------\n" << block << std::endl;
}

File::~File(void)
{
	_file.close();
}