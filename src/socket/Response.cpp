#include "Response.hpp"

Response::Response(void) : _config(NULL)
{
}

void Response::init(char *buff, Config *config)
{
    this->_config = config;
    this->_req.init(buff);
}

std::string Response::something(void)
{
    return _req.get_request_line();
}


void Response::open_page(void)
{
}

std::string Response::get_content(void)
{
    return "";
}

std::string Response::create_response(void)
{
    return "";
} 



std::string Response::get(void)
{
    std::string msg = "HTTP/1.1 200\r\ncontent-type: text/html; charset=utf-8\r\n\r\n";
    msg += this->_config->port.get();
    return msg;
}