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
		int rsize = recv(fd, buffer, 100, MSG_DONTWAIT);
		if (rsize > 0) {
			buffer[rsize] = '\0';
			std::cout << "Response: " << buffer << std::endl;
			char msg[] = "hi from client ^^";
			send(fd, msg, strlen(msg), MSG_DONTWAIT);
			close(fd);
			freeaddrinfo(addr);
			break ;
		}

	}
	return (0);
}