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

File::File(std::string file_name) : _file(file_name.c_str())
{
	if (this->_file.bad())
		throw std::runtime_error("failed to open '" + file_name + "'");
	std::string aux[] = {
		"host",
		"body_size",
		"root",
		"port",

		"save_files_path",
		"autoindex",
		"return"
	};

	for (int i = 0; i < 7; i++)
		single_value_keys.push_back(aux[i]);

	std::string aux2[] = {
		"server_name",
		"try_files",

		"methods",
		"try_files",
		"cgi_extensions",

	};

	for (int i = 0; i < 5; i++)
		multi_value_keys.push_back(aux2[i]);
 
	fill_data();
	extract_blocks();
	parse_blocks();
}

void File::fill_data(void)
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
		_data.erase();
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
}

void File::extract_blocks(void)
{
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



bool File::parse_single_value(std::string & block, std::map<std::string, std::string> & map)
{
	std::stringstream ss(block);
	std::string word;

	ss >> word;
	std::string line = block.substr(block.find(word) + word.length());
	line = line.substr(0, line.find_first_of('\n'));
	std::stringstream ss_line(line);
	ss_line >> map[word];
	std::string aux;
	ss_line >> aux;
	if (aux.empty() == false)
		throw std::runtime_error("'" + word + "' accepts just one value");
	block = block.substr(block.find(line) + line.length());
	return true;
}

bool File::is_inside(std::vector<std::string> & arr, std::string & str)
{
	std::vector<std::string>::iterator it;
	it = std::find(arr.begin(), arr.end(), str);
	return it != arr.end();
}

bool File::parse_multi_value(
	std::string & block, 
	std::map<std::string, std::list<std::string> > & map
)
{
	std::stringstream ss(block);
	std::string word;

	ss >> word;
	std::string line = block.substr(block.find(word) + word.length());
	line = line.substr(0, line.find_first_of('\n'));
	std::stringstream ss_line(line);
	std::string value;

	while (true)
	{
		ss_line >> value;
		if (value.empty())
			break ;
		map[word].push_back(value);
		value.erase();
	}
	block = block.substr(block.find(line) + line.length());
	return true;
}

bool File::parse_route(std::string & block)
{
	std::stringstream ss(block);
	std::string word;

	ss >> word;
	if (word != "location")
		throw std::runtime_error("'location' not found");
	ss >> word;
	if (word == "{")
		throw std::runtime_error("path not found");

	Conf route;
	route._single_value["location"] = word;
	
	ss >> word;
	if (word != "{")
		throw std::runtime_error("'{' not found");

	int start = block.find("{");
	std::string route_block = block.substr(
		start + 1,
		block.find("}") - start - 1
	);

		while (true)
		{
			std::string word;
			std::stringstream ss(route_block);
			ss >> word;
			if (word.empty())
				break ;
			if (is_inside(single_value_keys, word))
				parse_single_value(route_block, route._single_value);
			else if (is_inside(multi_value_keys, word))
				parse_multi_value(route_block, route._multi_values);
			else
				throw std::runtime_error("'" + word + "' is invalid");
		}
		this->routes.push_back(route);

		block = block.substr(block.find("}") + 1);
		return true;
}

void File::parse_blocks(void)
{
	while (_server_configs.empty() == false)
	{
		std::string block = _server_configs.front();
		_server_configs.pop_front();
		Conf config;
		while (true)
		{
			std::string word;
			std::stringstream ss(block);
			ss >> word;
			if (word.empty())
				break ;
			if (is_inside(single_value_keys, word))
				parse_single_value(block, config._single_value);
			else if (is_inside(multi_value_keys, word))
				parse_multi_value(block, config._multi_values);
			else if (word == "location")
				parse_route(block);
			else
				throw std::runtime_error("'" + word + "' is invalid");
		}
		this->confs.push_back(config);
	}
}

void File::info(std::list<Conf> & confs) const
{
	std::list<Conf>::const_iterator it;
	it = confs.begin();
	for (int i = 0; it != confs.end(); it++)
	{
		Conf const & conf = (*it);
		std::cout << "\n----------------- CONFIG " << i << " ----------------" << std::endl;
		std::cout << "                SINGLE-VALUES\n";
		std::map<std::string, std::string>::const_iterator it;
		it = conf._single_value.begin();
		for (; it != conf._single_value.end(); it++)
			std::cout << (*it).first << ": " << (*it).second << std::endl;
		
		std::cout << "\n               MULTI-VALUES\n";
		std::map<std::string, std::list<std::string> >::const_iterator it2;
		it2 = conf._multi_values.begin();
		for (; it2 != conf._multi_values.end(); it2++)
		{
			std::cout << (*it2).first << ": ";
			std::list<std::string> const & list = (*it2).second;
			std::list<std::string>::const_iterator it3;
			it3 = list.begin();
			for (; it3 != list.end(); it3++)
				std::cout << (*it3) << " ";
			std::cout << std::endl;
		}
		i++;
	}

}

File::~File(void)
{
	_file.close();
}