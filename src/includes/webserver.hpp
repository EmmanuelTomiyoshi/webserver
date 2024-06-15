#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include "./base.hpp" //all std libs
# include "ft.hpp"
#include "../socket/IPResolver.hpp"
#include "../utils/Utils.hpp"

#include "colors.hpp"

#define SOCKET_ERROR -1

void webtest(void);
int server_start(void);
void config_test(void);

#endif // WEBSERVER_HPP