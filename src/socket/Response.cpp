#include "Response.hpp"

std::string Response::http_version = "HTTP/1.1";

std::map<std::string, std::string> Response::mime_types;

Response::Response(char *buff, Config *config) : _http_response(NULL), _config(config)
{
    this->_req.init(buff);

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
    if (this->_type == "image")
        this->_file.open(this->_path.c_str(), std::ios::binary);
    else
        this->_file.open(this->_path.c_str());

    if (this->_file.bad())
        throw std::runtime_error("open_file: fail");
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
    //TODO: error();
}

void Response::set_public_file_info(void)
{
    this->_path = '.' + _req.get_target();
    if (_path.empty())
    {
        std::cerr << "error: no path provided" << std::endl;
        return ;
    }

    this->_ext = _path.substr(_path.find_last_of('.') + 1);
    if (_ext.empty())
    {
        std::cerr << "error: file without extension" << std::endl;
        return ;
    }

    this->_mime = mime_types[_ext];
    if (_mime.empty())
    {
        std::cerr << "error: mime type not allowed" << std::endl;
        return ;
    }

    this->_type = _mime.substr(0, _mime.find_first_of('/'));
}

void Response::show_file_info(void)
{
    std::cout << "----- FILE INFO ------\n";
    std::cout << "ext: " << this->_ext << std::endl;
    std::cout << "mime: " << this->_mime << std::endl;
    std::cout << "path: " << this->_path << std::endl;
    std::cout << "type: " << this->_type << std::endl;
}

void Response::open_file(void)
{
    if (Response::is_public())
    {
        set_public_file_info();
        open_public_file();
        return ;
    }

    try
    {
        _route = &(_config->routes.get(_req.get_target()));
        open_route_file();
    }
    catch (std::exception & e)
    {
        std::cerr << "page not found" << std::endl;
        return ;
        //error()
    }
}

Response::Body::Body(void)
{
    this->data = NULL;
}

Response::Body::~Body(void)
{
    if (this->data)
        delete[] this->data;
}

void Response::read_binary(void)
{
    if (_file.bad())
        return ;

    std::streamsize size = ft::get_file_size(_file);
    std::streambuf *buff = _file.rdbuf();
    char *data = new char[size];
    buff->sgetn(data, size);
    _body.data = data;
    _body.size = size;
}

void Response::read_text(void)
{
    if (!_file)
        throw std::runtime_error("read_text: bad_file");
    
    std::string text((std::istreambuf_iterator<char>(_file)),
                     std::istreambuf_iterator<char>());

    _body.size = text.size() + 1;
    _body.data = new char[_body.size];
    std::memmove(_body.data, text.c_str(), _body.size);
}

void Response::fill_body(void)
{
    if (_type == "image")
        read_binary();
    else
        read_text();
}

/* TODO:
    try to do all the standar operations
    if anything in the process goes wrong throw an error with a code
    get the code and send an error response

    try
    {
        operations()
    }
    catch ()
    {
        build_response_error()
    }

    send_response(_http_response);
 */

void Response::create_response(void)
{
    std::string status_code("200");
    std::string status_line = http_version + " " + status_code + " \r\n";
    std::string content_type = "Content-Type: " + _mime + "\r\n";
    std::string content_length = "Content-Length: " + 
        ft::int_to_str(_body.size) + "\r\n";

    std::string response = status_line + 
        content_type + 
        content_length +
        "\r\n";
    
    //obs: should or shouldn't put '\0' after the response data (before the body data)?
    size_t size = response.size();
    size += _body.size;
    _http_response = new char[size];
    _http_response_size = size;
    std::memmove(_http_response, response.c_str(), response.size());
    std::memmove(_http_response + response.size(), _body.data, _body.size);
}

void Response::GET(void)
{
    open_file();
    fill_body();
    create_response();
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

ssize_t Response::send_response(int fd)
{
    GET();
    return send(
        fd, 
        _http_response, 
        _http_response_size,
        MSG_DONTWAIT
    );
}

std::string Response::process(void)
{
    GET();
    return this->_config->port.get();
}


/*
request-line
headers

body
 */