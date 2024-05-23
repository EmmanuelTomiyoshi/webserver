#include "CGI.hpp"

CGI::CGI(void) : _body(NULL), _response_size(0)
{
    for (int i = 0; i < ENVS_SIZE; i++)
        _envs[i] = NULL;
    for (int i = 0; i < ARGV_SIZE; i++)
        _argv[i] = NULL;
}

void CGI::add_env(const char *value)
{
    if (!value)
        return ;

    for (int i = 0; i < 9; i++)
    {
        if (_envs[i] == NULL)
        {
            _envs[i] = value;
            break ;
        }
    }
}

void CGI::set_request_method(std::string value)
{
    _request_method = "REQUEST_METHOD=" + value;
    _request_method_raw = value;
    add_env(_request_method.c_str());
}

void CGI::set_content_type(std::string value)
{
    _content_type = "CONTENT_TYPE=" + value;
    add_env(_content_type.c_str());
}

void CGI::set_content_length(std::string value)
{
    _content_length = "CONTENT_LENGTH=" + value;
    _content_length_raw = value;
    add_env(_content_length.c_str());
}

void CGI::set_query_string(std::string value)
{
    _query_string = "QUERY_STRING=" + value;
    add_env(_query_string.c_str());
}

void CGI::set_script_name(std::string value)
{
    _script_name = "SCRIPT_FILENAME=" + value;
    _script_name_raw = value;
    add_env(_script_name.c_str());
    _argv[0] = _script_name_raw.c_str();
    _argv[1] = NULL;
}

void CGI::set_body(char *value)
{
    _body = value;
}


bool CGI::error(void)
{
    if (_argv[0] == NULL)
    {
        std::cerr << "CGI ERROR: argv empty" << std::endl;
        return true;
    }
    if (_envs[0] == NULL)
    {
        std::cerr << "CGI ERROR: envs empty" << std::endl;
        return true;
    }
    if (_request_method_raw == "POST")
    {
        if (_body == NULL)
        {
            std::cerr << "CGI ERROR: body empty" << std::endl;
            return true;
        }
        if (_content_length.empty())
        {
            std::cerr << "CGI ERROR: content_length empty" << std::endl;
            return true;
        }
    }

    return false;
}

void CGI::info(void)
{
    std::cout << "----ENVS----\n";
    for (int i = 0; _envs[i]; i++)
    {
        std::cout << _envs[i] << std::endl;
    }
    std::cout << "\n----ARGV----\n";
    for (int i = 0; _argv[i]; i++)
    {
        std::cout << _argv[i] << std::endl;
    }
}

void CGI::dup_pfds(void)
{
    //this program will write on _pfds_a[1] and the cgi script will read on _pfds_a[0]
    dup2(_pfds_a[0], STDIN_FILENO);
    close(_pfds_a[0]);
    //the cgi script will write on _pfds_b[1] and this program will read on _pfds_b[0]
    dup2(_pfds_b[1], STDOUT_FILENO);
    close(_pfds_b[1]);
}

void CGI::write_body(void)
{
    if (_request_method_raw != "POST")
        return ;
    size_t size = ft::str_to_int(_content_length_raw);
    std::cerr << "body_size_raw: " << size << std::endl;
    write(_pfds_a[1], _body, size);
}

void CGI::read_response(void)
{
    char buff[6000];
    _response_size = read(_pfds_b[0], buff, 6000);
    _response = new char[_response_size];
    std::memmove(_response, buff, _response_size);
}

void CGI::execute_cgi_script(void)
{
    pipe2(_pfds_a, O_NONBLOCK);
    pipe(_pfds_b);
    _pid = fork();
    if (_pid == 0)
    {
        dup_pfds();
        execve(_argv[0], (char * const *) _argv, (char * const *) _envs);
        std::cerr << "CGI ERROR: fail to execute cgi script" << std::endl;
        exit(0);
    }
    write_body();
    wait(NULL);
    read_response();
    close(_pfds_a[0]);
    close(_pfds_b[0]);
    close(_pfds_a[1]);
    close(_pfds_b[1]);
}

void CGI::extract_content_type(size_t header_size)
{
    char str[] = "Content-Type: ";
    size_t pos;
    for (size_t i = 0; i < header_size; i++)
    {
        if (header_size - i < std::strlen(str))
            return ;
        if (std::strncmp(str, &_response[i], std::strlen(str)))
        {
            pos = i + std::strlen(str) - 1;
            break ;
        }
    }
    char *content = &_response[pos];
    char *end = std::strchr(content, '\n');
    size_t size = end - content;
    char *content_type = new char[size + 1];
    std::memmove(content_type, content, size);
    content_type[size] = '\n';
    _response_data.content_type = content_type;
    delete [] content_type;
}

void CGI::extract_response_data(void)
{
    char *body = ft::get_body_position(_response, _response_size);
    if (body == NULL)
        throw std::runtime_error(HTTP_SERVICE_UNAVAILABLE);
    size_t header_size = body - _response;
    size_t body_size = _response_size - header_size;
    _response_data.body = body;
    _response_data.body_size = body_size;
    extract_content_type(header_size);
}

void CGI::format_http_response(void)
{
    std::string status_line = "HTTP/1.1 200 OK\n";
    std::string h1 = "Content-Length: " +
        ft::int_to_str(_response_data.body_size) + 
        "\n";
    std::string h2 = "Content-Type: " + 
        _response_data.content_type +
        "\n\n";
    

    std::string aux = status_line + h1 + h2;

    size_t size = aux.length() + _response_data.body_size;
    char *http_response = new char[size];
    std::memmove(http_response, aux.c_str(), aux.length());
    std::memmove(
        http_response + aux.length(),
        _response_data.body,
        _response_data.body_size
    );

    _response_data.http_response = http_response;
    _response_data.http_response_size = size;
}

void CGI::execute(void)
{
    if (this->error())
        return ;

    execute_cgi_script();
    extract_response_data();
    format_http_response();
}

char *CGI::get_response(void)
{
    return _response_data.http_response;
}

size_t CGI::get_response_size(void)
{
    return _response_data.http_response_size;
}
