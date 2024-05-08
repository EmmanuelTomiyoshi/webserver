#include "Response.hpp"

Response::Response(void) : _config(NULL)
{
}

/* 
    there will be 3 functions to process the request: GET(), POST(), DELETE()
    a map containing each of the functions mapped to the strings "GET", "POST", "DELETE"
    a function error() to send an error as a response if any operation fails
    when entering the GET, POST, DELETE function the first thing to be done is to get the route
    then verify if the METHOD is allowed by the route
    then process the request

    GET:
        read the file
        store in a string
        put in the body of the http response
        send to user

    POST:
        create a file with the requested format
        extract the body from the http request
        store the extracted body into this file
        send a successfull response to the user

    DELETE
        deletes a file inside the folder that contains user uploaded files
        sends a response back 

 */

void Response::GET(void)
{

}

void Response::POST(void)
{

}

void Response::DELETE(void)
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



std::string Response::process(void)
{
    std::string msg = "HTTP/1.1 200\r\ncontent-type: text/html; charset=utf-8\r\n\r\n";
    msg += this->_config->port.get();
    return msg;
}