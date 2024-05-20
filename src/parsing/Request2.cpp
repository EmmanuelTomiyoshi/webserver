#include "Request2.hpp"

Request2::Request2(void) : _buff(NULL), _body(NULL)
{
}

void Request2::init(char *buff)
{
    _buff = buff;
	separate_info();
    extract_request_line();
    extract_headers();
    extract_body_size();
    info();
}

void Request2::verify_initialization(void) const
{
    if (_buff == NULL)
    {
        std::cerr << "Request2 not initialized" << std::endl;
        throw std::runtime_error("Request2 not initialized");
    }
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

    if (_method.empty() || _target.empty())
        throw std::runtime_error(HTTP_BAD_REQUEST);
    if (_http_version != "HTTP/1.1")
        throw std::runtime_error(HTTP_BAD_REQUEST);
    if (_method != "GET" && _method != "POST" && _method != "DELETE")
        throw std::runtime_error(HTTP_METHOD_NOT_ALLOWED);

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

void Request2::extract_body_size(void)
{
    try
    {
        std::string & length = _headers.at("Content-Length");
        _body_size = ft::str_to_int(length);
    }
    catch (std::exception & e)
    {
        //Method POST requires a Content-Length header
        if (_method == "POST")
            throw std::runtime_error(HTTP_BAD_REQUEST);
        _body_size = 0;
        _body = NULL;
    }
}

char *Request2::get_body(void)
{
    verify_initialization();
    return _body;
}

size_t Request2::get_body_size(void)
{
    verify_initialization();
    return _body_size;
}

std::string Request2::get_header(std::string key) const
{
    verify_initialization();
    return _headers.at(key);
}

std::string Request2::get_method(void)
{
    verify_initialization();
    return this->_method;
}

std::string Request2::get_target(void)
{
    verify_initialization();
    return this->_target;
}

std::string Request2::get_version(void)
{
    verify_initialization();
    return this->_http_version;
}

void Request2::info(void)
{
    verify_initialization();
    std::cout << "----- Request2 INFO----\n" << _info << std::endl;
    std::cout << "Method: " << _method << std::endl;
    std::cout << "Target: " << _target << std::endl;
    std::cout << "Version: " << _http_version << std::endl;
    std::cout << "----- HEADERS -----\n" << std::endl;
    std::map<std::string, std::string>::iterator it;
    for (it = _headers.begin(); it != _headers.end(); it++)
        std::cout << (*it).first << " -> " << (*it).second << std::endl;
}