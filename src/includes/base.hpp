#ifndef BASE_HPP
# define BASE_HPP


#include <netinet/in.h>			// for htonl, htons, ntohl, ntohs
#include <sys/socket.h>			// for socket-related functions
#include <iostream>				// for I/O operations
#include <sys/types.h>			// for types used in system calls
#include <netdb.h>				// for network-related functions like getaddrinfo()
#include <arpa/inet.h>			// for manipulating IP addresses
#include <cstring>				// for c string manipulation functions
#include <errno.h>				// for error macros
#include <stdexcept>
#include <unistd.h>				// close()
#include <error.h>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <sys/epoll.h>
#include <fstream>
#include <fcntl.h>
#include <iterator>
#include "sys/wait.h"


# include <map>
# include <vector>
# include <list>
# include <algorithm>
# include <ctime>

# include "../tests/save_request.hpp"

#endif