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

#include <sstream>

#include "colors.hpp"
#include "IPResolver.hpp"

#define SOCKET_ERROR -1

void webtest();


#endif // WEBSERVER_HPP