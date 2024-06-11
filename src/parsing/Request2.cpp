#include "Request2.hpp"

Request2::Request2(void) : _buff(NULL), _body(NULL), _is_cgi(false)
{
}

void Request2::init(char *buff, ssize_t size)
{
    if (size <= 0)
    {
        std::cerr << "ERROR: Empty request" << std::endl;
        throw std::runtime_error(HTTP_BAD_REQUEST);
    }
    _buff = buff;
    _buff_size = size;
	separate_info();
    extract_request_line();
    extract_headers();
    extract_body();
    extract_route();
    extract_file();
    extract_query();
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
	char *body = ft::get_body_position(_buff, _buff_size);
	size_t size_info = body - _buff;
	char *info = new char[size_info + 1];
	std::memmove(info, _buff, size_info);
	info[size_info] = '\0';

    _info = info;
    _info_raw = info;
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
    if (_method != "GET" && _method != "POST" && _method != "DELETE" &&  _method != "OPTIONS")
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

        //remove '\r':
        if (value.find_first_of('\r') != std::string::npos)
            value = value.substr(0, value.find_first_of('\r'));

        _info = _info.substr(value_end + 1);
        _headers[key] = value;
    }
}

void Request2::extract_body(void)
{
    try
    {
        std::string & length = _headers.at("Content-Length");
        _body_size = ft::str_to_int(length);

        _body_bytes = _buff_size - _info_raw.size();
        _body = new char[_body_size];
        std::memmove(
            _body,
            ft::get_body_position(_buff, _buff_size),
            _body_bytes
        );
    }
    catch (std::exception & e)
    {
        //Method POST requires a Content-Length header
        if (_method == "POST")
            throw std::runtime_error(HTTP_BAD_REQUEST);
        _body_size = 0;
        _body_bytes = 0;
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
    try
    {
        return _headers.at(key);
    }
    catch (std::exception & e)
    {
        throw std::runtime_error(HTTP_BAD_REQUEST);
    }
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
    std::cout << "Route: " << _route << std::endl;
    std::cout << "File: " << _file << std::endl;
    std::cout << "Query: " << _query << std::endl;
    std::cout << "Is CGI: " << (_is_cgi ? "true" : "false") << std::endl;
    std::cout << "----- HEADERS -----\n" << std::endl;
    std::map<std::string, std::string>::iterator it;
    for (it = _headers.begin(); it != _headers.end(); it++)
        std::cout << (*it).first << " -> " << (*it).second << std::endl;
}

static bool is_file_or_query(std::string str)
{
    if (str.find_last_of('/') != std::string::npos)
        str = str.substr(str.find_last_of('/') + 1);
    
    if (str.find('.') != std::string::npos)
        return true;
    if (str.find('?') != std::string::npos)
        return true;
    return false;
}

void Request2::extract_route_normal(void)
{
    std::string r = _target;

    if (r.size() == 1)
    {
        _route = r;
        return ;
    }
    
    if (is_file_or_query(r))
    {
        r = r.substr(0, r.find_last_of('/'));
        if (r.empty())
            r = "/";
    }
    _route = r;
}

void Request2::extract_route_delete(void)
{
    std::string r = _target;

    if (r.size() == 1)
        throw std::runtime_error(HTTP_BAD_REQUEST);
    
    r = r.substr(0, r.find_last_of('/'));
    if (r.empty())
        _route = "/";
    else
        _route = r;
}

void Request2::extract_route(void)
{
    if (_method == "DELETE" || _method == "OPTIONS")
        extract_route_delete();
    else
        extract_route_normal();
}

void Request2::extract_file_cgi(void)
{
    std::string str = _target;

    str = str.substr(str.find_last_of('/') + 1);

    if (str.find_last_of('.') == std::string::npos)
        return ;
    _is_cgi = true;
    if (str.find_first_of('?') == std::string::npos)
    {
        _file = str;
        return ;
    }
    str = str.substr(0, str.find_first_of('?'));
    _file = str;
}

void Request2::extract_file_delete(void)
{
    std::string str = _target;

    str = str.substr(str.find_last_of('/') + 1);
    if (str.empty())
        throw std::runtime_error(HTTP_BAD_REQUEST);

    if (str.find_first_of('?') == std::string::npos)
    {
        _file = str;
        return ;
    }
    str = str.substr(0, str.find_first_of('?'));
    _file = str;
}

void Request2::extract_file(void)
{
    if (_method == "DELETE")
        extract_file_delete();
    else
        extract_file_cgi();
}

std::string Request2::get_file(void)
{
    verify_initialization();
    return _file;
}
std::string Request2::get_route(void)
{
    verify_initialization();
    return _route;
}
bool Request2::is_cgi(void)
{
    verify_initialization();
    return _is_cgi;
}

void Request2::extract_query(void)
{
    if (_target.find('?') == std::string::npos)
        return ;
    _query = _target.substr(_target.find_first_of('?') + 1);
}

std::string Request2::get_query(void)
{
    return _query;
}

void Request2::debug(void)
{
    std::cout << "\n-------- Request2 Debug --------" << std::endl;
    std::cout << "buff_size: " << _buff_size << std::endl;
    std::cout << "body_bytes: " << _body_bytes << std::endl;
    std::cout << "body_size: " << _body_size << std::endl;
    std::cout << "info_size: " << this->_info_raw.size() << std::endl;
    std::cout << "is_body_complete: " << (is_body_complete() ? "true" : "false") << std::endl;
    std::cout << "body_bytes_remaining: " << this->body_bytes_remaining() << std::endl;
}

ssize_t Request2::body_bytes_remaining(void)
{
    return _body_size - _body_bytes;
}

bool Request2::is_body_complete(void)
{
    return _body_bytes >= _body_size;
}

void Request2::add_more_body(char *buff, ssize_t size)
{
    std::memmove(_body + _body_bytes, buff, size);
    _body_bytes += size;
}
