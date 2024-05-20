#include "Request2.hpp"

Request2::Request2(char *buff) : _buff(buff), _body(NULL)
{
	separate_info();
    extract_request_line();
    extract_headers();
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

void Request2::extract_headers(void)
{
    while (1)
    {
        size_t key_end = _info.find_first_of(':');
        if (key_end == std::string::npos)
            return ;
        std::string key = _info.substr(0, key_end);

        size_t value_end = _info.find_first_of('\n');
        if (value_end == std::string::npos)
            return ;
        std::string value = _info.substr(key_end + 2, value_end - key_end - 2);

        _info = _info.substr(value_end + 1);
        _headers[key] = value;
    }
}

std::string Request2::get_header(std::string key) const
{
    return _headers.at(key);
}

std::string Request2::get_method(void)
{
    return this->_method;
}

std::string Request2::get_target(void)
{
    return this->_target;
}

std::string Request2::get_version(void)
{
    return this->_http_version;
}

void Request2::info(void)
{
    std::cout << "----- Request2 INFO----\n" << _info << std::endl;
    std::cout << "Method: " << _method << std::endl;
    std::cout << "Target: " << _target << std::endl;
    std::cout << "Version: " << _http_version << std::endl;
    std::cout << "----- HEADERS -----\n" << std::endl;
    std::map<std::string, std::string>::iterator it;
    for (it = _headers.begin(); it != _headers.end(); it++)
        std::cout << (*it).first << " -> " << (*it).second << std::endl;
}