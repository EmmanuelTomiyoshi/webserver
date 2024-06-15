#include "tests.hpp"

Request2 _request;
Configs configs("./conf/ws.conf");

ssize_t read_debug(size_t index, char **buff)
{
    std::string file("./src/tests/requests/request_test_");
    file += ft::int_to_str(index);
    std::cout << "file: " << file << std::endl;
    int fd = open(file.c_str(), O_RDONLY);
    ssize_t bytes = ft::read_all(fd, buff);
    close(fd);
    return bytes;
}

static void test_config(void)
{
    Config & config = *(configs.get().begin());
    config.show();
}

static void test_request(void)
{
    std::cout << "****TEST_REQUEST****" << std::endl;
    Config & config = *(configs.get().begin());

    char *buff;
    ssize_t bytes = read_debug(0, &buff);
    write(1, buff, bytes);
    _request.init(buff, bytes, &config);
    _request.info();
}

static void test_cgi(void)
{
    if (_request.is_cgi() == false)
        return ;
    Config & config = configs.next();
    std::cout << "\n------- CGI DEBUGGING --------" << std::endl;
    struct epoll_event event;
    CGI cgi;
    cgi.set_request_method("GET");
    cgi.set_body(_request.get_body());
    cgi.set_body_size(_request.get_body_size());
    cgi.set_query_string(_request.get_query());
    std::string script = config.routes.get(_request.get_route()).get_path();
    script += "/" + _request.get_file();
    std::cout << "Script path: " << script << std::endl;
    cgi.set_script_name(script);
    cgi.set_event(&event);
    cgi.execute();
    char *buff;
    ssize_t bytes = cgi.read_pfds_b(&buff);
    cgi.process_response(buff, bytes);

    ft::debug_file(
        "./src/tests/requests/debug_cgi_response", 
        cgi.get_response(), 
        cgi.get_response_size()
    );

    char *body = ft::get_body_position(cgi.get_response());
    ssize_t body_size = cgi.get_response_size() - (body - cgi.get_response());
    ft::debug_file(
        "./src/tests/requests/debug_index.html", 
        body,
        body_size
    );
}

void random_tests(void)
{
    std::cout << "FULL PATH: " << ft::get_full_path("./") << std::endl;
    std::cout << "FULL PATH: " << ft::get_full_path("./front") << std::endl;
    std::cout << "FULL PATH: " << ft::get_full_path("./cgi-bin/temp") << std::endl;
    std::cout << "FULL PATH: " << ft::get_full_path("/home/fodase/floeres/leguminosas") << std::endl;
    exit(0);
}

static void execute_tests(void)
{
    std::cout << "------------ EXECUTING TESTS ------------\n\n" << std::endl;
    random_tests();
    test_request();
    test_config();
    test_cgi();
}

#include "Memory.hpp"
#include "CustomData.hpp"
static void execute_tests2(void)
{
    char *buff = new char[100];
    char *buff2 = new char[100];
    char *buff3 = new char[100];
    char *buff4 = new char[100];
    Memory::add(buff);
    Memory::add(buff2);
    Memory::add(buff3);
    Memory::add(buff4);

    Memory::clear();

    epoll_event *event = new epoll_event;
    CustomData *data = new CustomData;
    data->request = new Request2;
    event->data.ptr = data;

    Memory::add(event);

    Memory::clear();
}

#include "BuffList.hpp"
void test_buff_list(void)
{
    BuffList list;
    char *buff = new char[5];
    std::strcpy(buff, "1234");
    list.add(buff, 5);

    std::cout << "VALUE: " << list.front().buff << std::endl;
    std::cout << "hello 1" << std::endl;
    std::cout << "hello 2" << std::endl;
}

void tests(int argc, char **argv)
{
    if (argc != 2)
        return ;

    if (std::string(argv[1]) == "test")
    {
        execute_tests();
        exit(0);
    }
    if (std::string(argv[1]) == "test2")
    {
        test_buff_list();
        exit(0);
        execute_tests2();
        exit(0);
    }
    if (std::string(argv[1]) == "debug")
    {
        set_save_request_true();
    }
}

namespace test 
{
}