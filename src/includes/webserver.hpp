#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

# include "./base.hpp" //all std libs
# include "../utils/ft.hpp"
#include "../parsing/Parser.hpp"
#include "../socket/IPResolver.hpp"
#include "../config/Methods.hpp"
#include "../config/Route.hpp"
#include "../config/Config.hpp"
#include "../config/Configs.hpp"
#include "../socket/Server.hpp"
#include "../utils/Utils.hpp"

#include "colors.hpp"

#define SOCKET_ERROR -1

void webtest(void);
int server_start(void);
void config_test(void);

#endif // WEBSERVER_HPP