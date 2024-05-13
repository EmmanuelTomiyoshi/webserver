#include "Response.hpp"

std::string Response::http_version = "HTTP/1.1";

std::map<std::string, std::string> Response::mime_types;

Response::Response(void) : _config(NULL)
{
    mime_types["js"] = "application/javascript";
    mime_types["json"] = "application/json";
    mime_types["pdf"] = "application/pdf";
    mime_types["html"] = "text/html; charset=UTF-8";
    mime_types["txt"] = "text/plain; charset=UTF-8";
    mime_types["css"] = "text/css; charset=UTF-8";
    mime_types["jpg"] = "image/jpeg";
    mime_types["jpeg"] = "image/jpeg";
    mime_types["png"] = "image/png";
    mime_types["gif"] = "image/gif";
    mime_types["bmp"] = "image/bmp";
    mime_types["svg"] = "image/svg+xml";
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
    std::string body,
    std::string mime = "text/html",
    size_t len = 0
)
{
    if (status_code.length() != 3)
        throw std::runtime_error("Status_code needs to be 3 digits. Got: " + status_code.length());
    if (ft::has_only_digits(status_code) == false)
        throw std::runtime_error("Status_code needs to have only digits");

    std::string status_line = http_version + " " + status_code + " \r\n";
    std::string content_type = "Content-Type: " + mime + "\r\n";
    if (len == 0)
        len = body.length();
    std::string content_length = "Content-Length: " + len + std::string("\r\n");


    std::string response = 
        status_line +
        content_type +
        content_length +
        "\r\n" + body;

    return response;
}

bool Response::is_public(void) {
    std::string str = _req.get_target();
    str = str.substr(0, std::string("/public").length());
    return str == "/public";
}

// std::string Response::GET(void)
// {
//     Route *route = NULL;

//     if (Response::is_public()) {
//         std::string path = '.' + _req.get_target();
//         std::ifstream file(path.c_str());
//         if (file.bad()) {
//             return build_response("404", "FILE NOT FOUND");
//         }
//         std::string ext = path.substr(path.find_last_of('.') + 1);
//         std::string mime = mime_types[ext];
//         if (mime.empty()) {
//             return build_response("401", "mime type not allowed");
//         }

//         if (ext == "png")
//         {
//             file.seekg(0, std::ios::end);
//             std::streampos fileSize = file.tellg();
//             file.seekg(0, std::ios::beg);
//             size_t s = static_cast<size_t>(fileSize) + 1;
//             char* imageData = new char[s];

//             file.read(imageData, fileSize);
//             imageData[fileSize] = '\0';
//             file.close();
//             return build_response("200", imageData, mime, fileSize);
//         }
//         std::string file_content = ft::read_file(file);
//         return build_response("200", file_content, mime);
//     }

//     try
//     {
//         route = &(_config->routes.get(_req.get_target()));
//     }
//     catch (std::exception & e)
//     {
//         //generate a not found error: error(NOT_FOUND)
//         std::cerr << "ROUTE NOT FOUND" << std::endl;
//         std::string path = "./public/pages/error.html";
//         std::ifstream file(path.c_str());
//          std::string file_content = ft::read_file(file);
//         if (file.bad()) {
//             return build_response("404", "FILE NOT FOUND");
//         }
//         return build_response("404", file_content);
//     }

//     if (route->methods.allowed("GET") == false)
//     {
//         //error(METHOD_NOT_ALLOWED)
//         std::cerr << "METHOD_NOT_ALLOWED" << std::endl;
//         return build_response("401", "METHOD NOT ALLOWED");
//     }

//     std::list<std::string>::const_iterator it;
//     it = route->try_files.get().begin();
//     std::ifstream file;
//     for (; it != route->try_files.get().end(); it++)
//     {
//         // std::string path = route->get_root() + '/' + (*it);
//         std::string path = "./public/pages/index.html";
//         file.open(path.c_str());
//         if (file.good())
//             break ;
//     }

//     if (it == route->try_files.get().end())
//     {
//         std::cout << "default page not found" << std::endl;
//         //error(DEFAULT_PAGE_NOT_FOUND)
//         return build_response("404", "DEFAULT PAGE NOT FOUND");
//     }

//     std::string file_content = ft::read_file(file);
//     std::string response = build_response(
//         "200",
//         file_content
//     );
//     return response;
// }

void Response::open_public_file(void)
{
    this->_path = '.' + _req.get_target();
    this->_file.open(this->_path.c_str());
    if (this->_file.bad())
        throw std::runtime_error("open_file: fail");
    std::cout << "opened public file" << std::endl;
}

void Response::open_route_file(void)
{
    std::list<std::string>::const_iterator it;
    it = _route->try_files.get().begin();
    for (; it != _route->try_files.get().end(); it++)
    {
        _path = _route->get_path() + '/' + (*it);
        _file.open(_path.c_str());
        if (_file.good())
            return ;
    }
    std::cout << "opened route file" << std::endl;
    //TODO: error();
}

void Response::open_file(void)
{
    if (Response::is_public())
        open_public_file();
    else
        open_route_file();
}

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
    return this->_config->port.get();
}


/*
request-line
headers

body
 */