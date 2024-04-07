#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

std::string http_messagqe(void) {
	return std::string(
		"GET /hello.txt HTTP/1.1\nUser-Agent: curl/7.64.1\r\nHost: www.example.com\nAccept-Language: en, mi"
	);
}

addrinfo get_hints(void) {
	addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	// hints.ai_socktype = SOCK_STREAM;
	// hints.ai_flags |= AI_CANONNAME;
	// hints.ai_protocol = 0;
	// hints.ai_next = NULL;
	// hints.ai_addr = NULL;
	// hints.ai_canonname = NULL;
	return hints;
}

void	addr_print(addrinfo *addr) {
	std::cout << "----- ADDR INFO -----" << std::endl;
	if (addr == NULL) {
		std::cout << "addr is empty" << std::endl;
		perror ("getaddrinfo");
	}
	else {
		std::cout << "SocketLen: " << addr->ai_addrlen << std::endl;
		std::cout << "Family: " << addr->ai_family << std::endl;
		std::cout << "Protocol: " << addr->ai_protocol << std::endl;
		std::cout << "SockType: " << addr->ai_socktype << std::endl;
		std::cout << "Flags: " << addr->ai_flags << std::endl;
	}
}

int main (void)
{
	addrinfo	hints = get_hints();
	addrinfo	*addr = NULL;
	// addrinfo	*rp = NULL;

	int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	getaddrinfo("localhost", "3005", &hints, &addr);
	connect(fd, addr->ai_addr, addr->ai_addrlen);
	
	while (1) {
		char buffer[100];
		if (read(fd, buffer, 100) > 0) {
			std::cout << "Response: " << buffer << std::endl;
		}

	}
	// socklen_t socklen;
	// int nfd = accept(fd, addr->ai_addr, &socklen);
	// std::cout << "nfd: " << nfd << std::endl;
	perror("listen");
	addr_print(addr);
	(void)fd;
	return (0);
}