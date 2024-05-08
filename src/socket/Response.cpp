#include "Response.hpp"

Response::Response(std::string request) : _req(request)
{
}

Response::Response(void)
{
}

void Response::init(char *buff)
{
    this->_req.init(buff);
}

Response::Response(void *ptr) :
_req(*(static_cast<std::string*>(ptr)))
{
    std::cout << "response created: " << _req.get_method() << std::endl;
}

std::string Response::something(void)
{
    return _req.get_request_line();
}

std::string Response::get(void)
{
    std::string msg = "HTTP/1.1 200\r\ncontent-type: text/html; charset=utf-8\r\n\r\n";
    msg += this->_req.get_target();
    return msg;
}