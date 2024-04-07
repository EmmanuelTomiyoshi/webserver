#include "webserver.hpp"
#include "./parsing/Request.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Wrong arguments number" << std::endl;
		return (1);
	}

	std::string str = std::string(argv[1]);
	if (str.empty())
	{
		std::cerr << "Argument must not be empty" << std::endl;
		return (1);
	}

	IPResolver ip;

	std::string hostname = "google.com";
	IPResolver resolver;
	resolver.printIPAdresses(hostname);

	addrinfo	hints = get_hints();
	addrinfo	*addr = NULL;
	// addrinfo	*rp = NULL;

	int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	getaddrinfo("localhost", "3005", &hints, &addr);
	bind(fd, addr->ai_addr, addr->ai_addrlen);
	listen(fd, 4);
	while (1) {
		int connFd = accept(fd, NULL, NULL);
		if (connFd != -1) {
			std::cout << "connected? " << std::endl;
		}
		send(connFd, "sera q vai?", 12, MSG_DONTWAIT);
	}
	// socklen_t socklen;
	// int nfd = accept(fd, addr->ai_addr, &socklen);
	// std::cout << "nfd: " << nfd << std::endl;
	perror("listen");
	addr_print(addr);
	(void)fd;
	return (0);

}