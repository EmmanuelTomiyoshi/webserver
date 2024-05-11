#include "Response.hpp"

std::string Response::http_version = "HTTP/1.1";

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

std::string Response::build_response(
    std::string status_code,
    std::string body
)
{
    if (status_code.length() != 3)
        throw std::runtime_error("Status_code needs to be 3 digits. Got: " + status_code.length());
    if (ft::has_only_digits(status_code) == false)
        throw std::runtime_error("Status_code needs to have only digits");

    std::string status_line = http_version + " " + status_code + " \r\n";
    std::string content_type = "Content-Type: text/html; charset=UTF-8\r\n";
    std::string content_length = "Content-Length: " + body.length() + std::string("\r\n");

    std::string response = 
        status_line +
        content_type +
        content_length +
        "\r\n" + body;

    return response;
}

std::string Response::GET(void)
{
    Route *route = NULL;

    try
    {
        route = &(_config->routes.get(_req.get_target()));
    }
    catch (std::exception & e)
    {
        //generate a not found error: error(NOT_FOUND)
        std::cerr << "ROUTE NOT FOUND" << std::endl;
        return build_response("404", "ROUTE NOT FOUND");
    }

    if (route->methods.allowed("GET") == false)
    {
        //error(METHOD_NOT_ALLOWED)
        std::cerr << "METHOD_NOT_ALLOWED" << std::endl;
        return build_response("401", "METHOD NOT ALLOWED");
    }

    std::list<std::string>::const_iterator it;
    it = route->try_files.get().begin();
    std::ifstream file;
    for (; it != route->try_files.get().end(); it++)
    {
        std::string path = route->get_root() + '/' + (*it);
        file.open(path.c_str());
        if (file.good())
            break ;
    }

    if (it == route->try_files.get().end())
    {
        std::cout << "default page not found" << std::endl;
        //error(DEFAULT_PAGE_NOT_FOUND)
        return build_response("404", "DEFAULT PAGE NOT FOUND");
    }

    std::string file_content = ft::read_file(file);
    std::string response = build_response(
        "200",
        file_content
    );
    return response;
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
    return this->GET();
}
