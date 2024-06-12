#ifndef SAVE_REQUEST_HPP
# define SAVE_REQUEST_HPP

#include "base.hpp"
#include "ft.hpp"

bool should_save_request(void);
void set_save_request_true(void);
void save_request(char *buff, ssize_t size);

#endif