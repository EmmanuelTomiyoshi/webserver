#include "CGI.hpp"

// static void execute_script(char **argv, char **envs)
// {
//     int pid = fork();
//     if (pid == 0)
//     {
//         execve(argv[0], argv, envs);
//         exit(0);
//     }
//     wait(NULL);
// }

// char *cgi(
//     char *content_type, 
//     char *request_method, 
//     char *script_name
// )
// {
//     char *argv[] = {
//         cgi_program,
//         script_name,
//         NULL
//     };

//     char *envs[] = {
//         gateway_interface,
//         server_protocol,
//         "CONTENT_TYPE=text/html",
//         // "CONTENT_LENGTH=200",
//         "REQUEST_METHOD=GET",
//         "SCRIPT_NAME=./cgi-bin/hello.py",
//         NULL
//     };

//     execute_script(argv, envs);
// }

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
    _script_name = "SCRIPT_NAME=" + value;
    _script_name_raw = value;
    add_env(_script_name.c_str());
    _argv[0] = CGI_PROGRAM;
    _argv[1] = _script_name_raw.c_str();
    _argv[2] = NULL;
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

void CGI::execute_get(void)
{
    
}

void CGI::execute_post(void)
{

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
    write(_pfds_a[1], _body, size);
}

void CGI::read_response(void)
{
    char buff[6000];
    _response_size = read(_pfds_b[0], buff, 6000);
    _response = new char[_response_size];
    std::memmove(_response, buff, _response_size);
}

void CGI::execute(void)
{
    if (this->error())
        return ;

    pipe(_pfds_a);
    pipe(_pfds_b);
    _pid = fork();
    if (_pid == 0)
    {
        dup_pfds();
        execve(_argv[0], (char * const *) _argv, (char * const *) _envs);
        std::cerr << "CGI ERROR: fail to execute cgi script" << std::endl;
        exit(0);
    }
    wait(NULL);
    read_response();
    close(_pfds_a[0]);
    close(_pfds_b[0]);
    close(_pfds_a[1]);
    close(_pfds_b[1]);
    write(1, _response, _response_size);
    std::cout << std::endl;
}

char *CGI::get_response(void)
{
    return _response;
}

size_t CGI::get_response_size(void)
{
    return _response_size;
}
