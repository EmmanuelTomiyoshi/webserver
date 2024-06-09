#include "Response.hpp"

std::string Response::http_version = "HTTP/1.1";

std::map<std::string, std::string> Response::mime_types;

Response::Response(char *buff, size_t size, Config *config, Timeout *timeout) : 
_buff(buff), _buff_size(size), _status("200"), _http_response(NULL),
_config(config), _timeout(timeout)
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

Response::Response(void)
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

bool Response::is_public(void) {
    std::string str = _request.get_target();
    str = str.substr(0, std::string("/public").length());
    return str == "/public";
}

void Response::open_public_file(void)
{
    if (this->_type == "image")
        this->_file.open(this->_path.c_str(), std::ios::binary);
    else
        this->_file.open(this->_path.c_str());

    if (!_file)
        throw std::runtime_error(HTTP_INTERNAL_SERVER_ERROR);
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
    this->_path = '.' + _request.get_target();
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
        _route = &(_config->routes.get(_request.get_target()));
        open_route_file();
    }
    catch (std::exception & e)
    {
        throw std::runtime_error(HTTP_NOT_FOUND);
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
    if (!_file)
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
        throw std::runtime_error(HTTP_INTERNAL_SERVER_ERROR);
    
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

void Response::build_error(std::string code)
{
    _status = code;
    _ext = "html";
    _mime = this->mime_types[_ext];
    _type = "html";
    _path = "./public/pages/errors/" + code + ".html";
}

void Response::create_response(void)
{
    std::string status_line = http_version + " " + _status + " \r\n";
    std::string content_type = "Content-Type: " + _mime + "\r\n";
    std::string content_length = "Content-Length: " + 
        ft::int_to_str(_body.size) + "\r\n";

    std::string response = status_line + 
        "Connection: close\r\n" +
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

void Response::GET_normal(void)
{
    open_file();
    fill_body();
    create_response();
    ft::CustomData *event_data = (ft::CustomData *) _event->data.ptr;
    send(
        event_data->fd,
        _http_response, 
        _http_response_size,
        MSG_DONTWAIT
    );
    epoll_ctl(event_data->epfd, EPOLL_CTL_DEL, event_data->fd, _event);
	close(event_data->fd);
}
void Response::GET_cgi(void)
{
    CGI cgi;
    cgi.set_request_method("GET");
    cgi.set_body(_request.get_body());
    cgi.set_body_size(_request.get_body_size());
    cgi.set_query_string(_request.get_query());
    cgi.set_timeout(_timeout);
    std::string script = _route->get_path();
    script += "/" + _request.get_file();
    cgi.set_script_name(script);
    cgi.set_event(_event);
    cgi.execute();
}

void Response::GET(void)
{
    if (!is_public() && _route->methods.allowed("GET") == false)
        throw std::runtime_error(HTTP_METHOD_NOT_ALLOWED);
    if (!is_public() && _route->cgi_route.is_true())
    {
        GET_cgi();
    }
    else
        GET_normal();
}

char *find_str_pos(const char *str, char *src)
{
    while (*src)
    {
        if (std::strncmp(str, src, std::strlen(str)) == 0)
            return src;
        src++;
    }
    return NULL;
}

void Response::POST(void)
{
    if (_route->methods.allowed("POST") == false)
        throw std::runtime_error(HTTP_METHOD_NOT_ALLOWED);
    CGI cgi;
    cgi.set_request_method("POST");
    cgi.set_body(_request.get_body());
    cgi.set_content_length(_request.get_header("Content-Length"));
    cgi.set_body_size(_request.get_body_size());
    cgi.set_content_type(_request.get_header("Content-Type"));
    cgi.set_timeout(_timeout);

    static int i = 0;
    i++;
    std::cout << "---> i: " << i << std::endl;
    if (i % 2 == 0)
        cgi.set_script_name("./cgi-bin/upload_debug.pl");
    else
        cgi.set_script_name("./cgi-bin/upload_debug_loop.pl");
    
    
    cgi.set_event(_event);
    cgi.info();
    cgi.execute();

    _http_response = cgi.get_response();
    _http_response_size = cgi.get_response_size();
}

void Response::DELETE(void)
{
    if (_route->methods.allowed("DELETE") == false)
        throw std::runtime_error(HTTP_METHOD_NOT_ALLOWED);
}

void Response::execute(void)
{
    std::string method = _request.get_method();
    if (method == "GET")
        GET();
    else if (method == "POST")
        POST();
    else if (method == "DELETE")
        DELETE();
    else
        throw std::runtime_error(HTTP_METHOD_NOT_ALLOWED);
}

void Response::execute_error(std::string code)
{
    build_error(code);
    open_public_file();
    fill_body();
    create_response();
    ft::CustomData *event_data = (ft::CustomData *) _event->data.ptr;
    send(
        event_data->fd,
        _http_response, 
        _http_response_size,
        MSG_DONTWAIT
    );
    epoll_ctl(event_data->epfd, EPOLL_CTL_DEL, event_data->fd, _event);
	close(event_data->fd);
}

ssize_t Response::send_response(epoll_event & event)
{
    ft::CustomData *event_data = (ft::CustomData *) event.data.ptr;
    _event = &event;

    try
    {
        std::cout << "send response ----" << std::endl;
        _request.init(_buff, _buff_size);
        std::cout << "is_public: " << (is_public() ? "true" : "false") << std::endl;
        if (is_public() == false)
        {
            _route = &(_config->routes.get(_request.get_route()));
            _route->show();
        }
        this->execute();
    }
    catch(const std::exception& e)
    {
        std::cout << "executing error: " << e.what() << std::endl;
        execute_error(e.what());
    }

    if (_request.get_method() == "POST")
    {
        std::cout << "*********JAMALAICACA" << std::endl;
        epoll_ctl(event_data->epfd, EPOLL_CTL_DEL, event_data->fd, &event);
        return 1;
    }
    return 1;
}

void Response::process_error(std::string code)
{
    build_error(code);
    open_public_file();
    fill_body();
    create_response();
}
char *Response::get_response(void)
{
    return _http_response;
}
ssize_t Response::get_response_size(void)
{
    return _http_response_size;
}
