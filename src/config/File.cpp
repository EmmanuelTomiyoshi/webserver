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

File::File(std::string file_name)
{
	std::fstream file(file_name);
	if (file.bad())
		throw std::runtime_error("failed to open '" + file_name + "'");
	char buffer[2000];
	while (!file.eof())
	{
		file.getline(buffer, 2000, '\0');
		this->_data += buffer;
	}
	file.close();
}

void File::read_config_block(void)
{

}

File::~File(void)
{
}