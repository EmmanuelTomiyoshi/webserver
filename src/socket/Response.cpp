#include "Response.hpp"
#include "CustomData.hpp"
#include "Memory.hpp"

std::string Response::http_version = "HTTP/1.1";

std::map<std::string, std::string> Response::mime_types;

Response::Response(void) : _request(NULL), _route(NULL),
_http_response(NULL), _config(NULL), _event(NULL)
{
}

Response::Response(epoll_event *event) : _route(NULL), _status("200"),
_http_response(NULL), _http_response_size(0)
{
	CustomData *event_data = (CustomData *) event->data.ptr;

    _event = event;
    _config = event_data->config;
    _request = event_data->request;
    start_mimes();
}

Response::~Response(void)
{
    if (_http_response != NULL)
        delete [] _http_response;
}

void Response::start_mimes(void)
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

bool Response::is_public(void) {
    std::string str = _request->get_target();
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

void Response::open_route_file_default(void)
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
    std::cerr << "ERROR: open_route_file_default" << std::endl;
    throw std::runtime_error(HTTP_NOT_FOUND);
}

void Response::open_route_file_upload(void)
{
    _path = _route->save_files_path.get() + '/' + _request->get_file();
    _file.open(_path.c_str());
    if (_file.good())
        return ;
    std::cerr << "ERROR: open_route_file_upload" << std::endl;
    throw std::runtime_error(HTTP_NOT_FOUND);
}

void Response::open_route_file(void)
{
    if (_request->get_file().empty())
        open_route_file_default();
    else
        open_route_file_upload();
}

void Response::set_public_file_info(void)
{
    this->_path = '.' + _request->get_target();
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
        open_route_file();
    }
    catch (std::exception & e)
    {
        throw std::runtime_error(HTTP_NOT_FOUND);
    }
}

Response::Body::Body(void) : data(NULL), should_free(true)
{
}

Response::Body::~Body(void)
{
    if (this->data && this->should_free)
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
    std::memmove((void*) _body.data, text.c_str(), _body.size);
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

void Response::replace_error_path(std::string code)
{
    if (_config->error_pages.get(code).empty() == false)
        _path = _config->error_pages.get(code);
    if (_route != NULL && _route->error_pages.get(code).empty() == false)
        _path = _route->error_pages.get(code);
}

void Response::build_default_error(void)
{
    _status = HTTP_INTERNAL_SERVER_ERROR;
    _ext = "html";
    _mime = this->mime_types[_ext];
    _type = "html";
    _body.data = (char *) DEFAULT_ERROR_MESSAGE;
    _body.size = std::strlen(_body.data);
    _body.should_free = false;
}

void Response::create_response(void)
{
    std::string status_line = http_version + " " + _status + " \r\n";
    std::string h1 = "Content-Type: " + _mime + "\r\n";
    std::string h2 = "Access-Control-Allow-Origin: *\r\n";
    std::string h3 = "Access-Control-Allow-Methods: GET, POST, DELETE\r\n";
    std::string content_length = "Content-Length: " + 
        ft::int_to_str(_body.size) + "\r\n";

    std::string response = status_line + 
        "Connection: close\r\n" +
        h1 + h2 + h3 + 
        content_length +
        "\r\n";
    
    size_t size = response.size() + _body.size;
    if (size <= 0)
        throw std::runtime_error(HTTP_NOT_FOUND);
    _http_response = new char[size];
    _http_response_size = size;
    std::memmove(_http_response, response.c_str(), response.size());
    std::memmove(_http_response + response.size(), _body.data, _body.size);
}

void Response::fill_mime(std::string file)
{
    if (file.find_last_of('.') == std::string::npos)
        return ;
    std::string ext = file.substr(file.find_last_of('.') + 1);
    _mime = mime_types[ext];
}

void Response::GET_normal(void)
{
    open_file();
    fill_mime(_path);
    fill_body();
    create_response();
    create_writing_event(_event, _http_response, _http_response_size);
    _http_response = NULL;
}

void Response::GET_cgi(void)
{
    CGI cgi;
    cgi.set_request_method("GET");
    cgi.set_body(_request->get_body());
    cgi.set_body_size(_request->get_body_size());
    cgi.set_query_string(_request->get_query());
    cgi.set_route(_route);
    std::string script = _route->get_path();
    script += "/" + _request->get_file();
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
    cgi.set_body(_request->get_body());
    cgi.set_content_length(_request->get_header("Content-Length"));
    cgi.set_body_size(_request->get_body_size());
    cgi.set_content_type(_request->get_header("Content-Type"));
    cgi.set_script_name("./cgi-bin/upload_debug.pl");
    cgi.set_route(_route);

    cgi.set_event(_event);
    cgi.info();
    cgi.execute();
}

void Response::DELETE(void)
{
    if (_route->methods.allowed("DELETE") == false)
        throw std::runtime_error(HTTP_METHOD_NOT_ALLOWED);

    std::string file_path = _route->save_files_path.get();
    file_path += "/" + _request->get_file();
    std::cout << "FILE_PATH: " << file_path << std::endl;
    if (ft::file_exists(file_path) == false)
        throw std::runtime_error(HTTP_NOT_FOUND);
    if (std::remove(file_path.c_str()) != 0)
        throw std::runtime_error(HTTP_INTERNAL_SERVER_ERROR);
    create_cors_response();
    CustomData *event_data = (CustomData *) _event->data.ptr;
    send(
        event_data->fd,
        _http_response, 
        _http_response_size,
        MSG_DONTWAIT
    );
    epoll_ctl(event_data->epfd, EPOLL_CTL_DEL, event_data->fd, _event);
	close(event_data->fd);
}

void Response::create_cors_response(void)
{
    std::string status_line = http_version + " 200 OK\r\n";
    std::string h0 = "Connection: close\r\n";
    std::string h1 = "Content-Length: 0\r\n";
    std::string h2 = "Access-Control-Allow-Origin: *\r\n";
    std::string h3 = "Access-Control-Allow-Methods: GET, POST, DELETE\r\n";

    std::string response = status_line + 
        h0 + h1 + h2 + h3 + "\r\n";
        

    size_t size = response.size();
    _http_response = new char[size];
    _http_response_size = size;
    std::memmove(_http_response, response.c_str(), response.size());
}

void Response::execute(void)
{
    std::string method = _request->get_method();
    std::cout << "&*****method: " << method << std::endl;
    if (method == "GET")
        GET();
    else if (method == "POST")
        POST();
    else if (method == "DELETE")
        DELETE();
    else if (method ==  "OPTIONS")
    {
        create_cors_response();
        CustomData *event_data = (CustomData *) _event->data.ptr;
        send(
            event_data->fd,
            _http_response, 
            _http_response_size,
            MSG_DONTWAIT
        );
        epoll_ctl(event_data->epfd, EPOLL_CTL_DEL, event_data->fd, _event);
        close(event_data->fd);
        Memory::del(_event);
    }
    else
        throw std::runtime_error(HTTP_METHOD_NOT_ALLOWED);
}

void Response::execute_error(std::string code)
{
    try
    {
        build_error(code);
        replace_error_path(code);
        open_public_file();
        fill_body();
    }
    catch (std::exception & e)
    {
        build_default_error();
    }
    create_response();
    CustomData *event_data = (CustomData *) _event->data.ptr;
    send(
        event_data->fd,
        _http_response, 
        _http_response_size,
        MSG_DONTWAIT
    );
    epoll_ctl(event_data->epfd, EPOLL_CTL_DEL, event_data->fd, _event);
	close(event_data->fd);
    Memory::del(_event);
}

void Response::autoindex(void)
{
    std::cout << "\n-> EXECUTING AUTOINDEX <-\n" << std::endl;

    CGI cgi;
    cgi.set_request_method("GET");
    cgi.set_query_string(_request->get_query());
    cgi.set_route(_route);
    cgi.set_script_name("./cgi-bin/autoindex.pl");
    cgi.set_event(_event);
    cgi.execute();
}

ssize_t Response::send_response(void)
{
    try
    {
        if(_request->is_error())
            throw std::runtime_error(_request->get_error());
        if (is_public() == false)
            _route = &_config->routes.get(_request->get_route());
        this->execute();
    }
    catch(const std::exception& e)
    {
        std::cout << "executing error: " << e.what() << std::endl;
        if (_route != NULL && e.what() == std::string(HTTP_NOT_FOUND) && _route->autoindex.get())
            autoindex();
        else
            execute_error(e.what());
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

void Response::create_writing_event(epoll_event *old_event, char *buff, ssize_t size)
{
    CustomData *old_data = (CustomData *) old_event->data.ptr;

    CustomData *data = new CustomData;
    data->fd = old_data->fd;
    data->buff = buff;
    data->buff_size = size;
    data->w_count = 0;
    data->type = ft::RESPONSE;
    data->config = old_data->config;
    data->duration = 5;
    data->epfd = old_data->epfd;
    data->start_time = time(NULL);

    epoll_event event;
    event.events = EPOLLOUT;
    event.data.ptr = data;

    epoll_ctl(old_data->epfd, EPOLL_CTL_MOD, old_data->fd, &event);
    Memory::add(data);
    Memory::del(old_event);
}
