#include "Request2.hpp"

Request2::Request2(char *buff) : _buff(buff), _body(NULL)
{
	separate_info();
    extract_request_line();
    info();
}

void Request2::separate_info(void)
{
	_body = ft::get_body_position(_buff);
	size_t size_info = _body - _buff;
	char *info = new char[size_info + 1];
	std::memmove(info, _buff, size_info);
	info[size_info] = '\0';
	_info = info;
	delete [] info;
}

void Request2::extract_request_line(void)
{
    std::string line = _info.substr(0, _info.find_first_of('\n'));
    std::stringstream ss(line);

    ss >> _method;
    ss >> _target;
    ss >> _http_version;

    _info = _info.substr(_info.find_first_of('\n') + 1);
}

void Request2::info(void)
{
    std::cout << "----- Request2 INFO----\n" << _info << std::endl;
    std::cout << "Method: " << _method << std::endl;
    std::cout << "Target: " << _target << std::endl;
    std::cout << "Version: " << _http_version << std::endl;
    std::cout << "----- INFO AGAIN -----\n" << _info << std::endl;
}