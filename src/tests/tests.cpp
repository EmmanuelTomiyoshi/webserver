#include "tests.hpp"

void test_request(void);

static void execute_tests(void)
{
    std::cout << "------------ EXECUTING TESTS ------------\n\n" << std::endl;
    test_request();
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