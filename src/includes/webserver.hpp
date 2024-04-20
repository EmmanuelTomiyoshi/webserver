#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <netinet/in.h>			// for htonl, htons, ntohl, ntohs
#include <sys/socket.h>			// for socket-related functions
#include <iostream>				// for I/O operations
#include <sys/types.h>			// for types used in system calls
#include <netdb.h>				// for network-related functions like getaddrinfo()
#include <arpa/inet.h>			// for manipulating IP addresses
#include <cstring>				// for c string manipulation functions
#include <errno.h>				// for error macros
#include <unistd.h>				// close()
#include <error.h>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <sys/epoll.h>
#include <fstream>
# include <map>
# include <vector>
# include <list>
# include <algorithm>

#include "../parsing/Request.hpp"
#include "../socket/IPResolver.hpp"
#include "../socket/Server.hpp"
#include "../config/Methods.hpp"
#include "../config/Route.hpp"
#include "../config/Config.hpp"
#include "../config/File.hpp"

#include "colors.hpp"

#define SOCKET_ERROR -1

void webtest(void);
int server_start(void);
void config_test(void);

#endif // WEBSERVER_HPP