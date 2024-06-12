#include "save_request.hpp"

static bool *store(void)
{
    static bool value = false;
    return &value;
}

bool should_save_request(void)
{
    return *(store());
}

void set_save_request_true(void)
{
    bool *value = store();
    *value = true;
}

void set_save_request_false(void)
{
    bool *value = store();
    *value = false;
}

void save_request(char *buff, ssize_t size)
{
    if (should_save_request())
    {
        ft::debug_file("./src/tests/requests/request_test", buff, size);
        set_save_request_false();
    }
}
