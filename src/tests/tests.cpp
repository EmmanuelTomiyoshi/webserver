#include "tests.hpp"

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

void test_config(void)
{
    Configs configs("./conf/ws.conf");

    Config & config = configs.next();
    config.show();
}

void test_request(void)
{
    std::cout << "****TEST_REQUEST****" << std::endl;
    Request2 request;

    char *buff;
    ssize_t bytes = read_debug(0, &buff);
    write(1, buff, bytes);
    request.init(buff, bytes);
    request.info();
}


static void execute_tests(void)
{
    std::cout << "------------ EXECUTING TESTS ------------\n\n" << std::endl;
    test_request();
    test_config();
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
    if (std::string(argv[1]) == "debug")
    {
        set_save_request_true();
    }
}

namespace test 
{
}