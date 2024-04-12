#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>

std::string http_messaqe(void) {
	return std::string(
		"GET /hello.txt HTTP/1.1\nUser-Agent: curl/7.64.1\r\nHost: www.example.com\nAccept-Language: en, mi"
	);
}

addrinfo get_hints(void) {
	addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	return hints;
}

int recv_message(int fd)
{
	char buffer[100];
	int rsize = recv(fd, buffer, 100, MSG_WAITALL);
	if (rsize > 0)
	{
		buffer[rsize] = '\0';
		std::cout << "MESSAGE RECEIVED: " << buffer << std::endl;	
	}
	else
		std::cout << "message not received!!!" << std::endl;
	return rsize;
}

int main (void)
{
	addrinfo	hints = get_hints();
	addrinfo	*addr = NULL;
	// addrinfo	*rp = NULL;

	int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	getaddrinfo("localhost", "3005", &hints, &addr);
	if (connect(fd, addr->ai_addr, addr->ai_addrlen) == 0)
		std::cout << "CONNECTED TO SERVER" << std::endl;
	std::string http;
	http = http_messaqe();
	int bsent = send(fd, http.c_str(), http.size(), MSG_DONTWAIT);
	if (bsent > 0)
		std::cout << "MESSAGE SENT SUCCESSFULLY" << std::endl;
	
	for (int i = 0; i < 10; i++)
	{
		sleep(1);
		char buff[100];
		int rsize = recv(fd, buff, 100, MSG_WAITALL);
		if (rsize > 0)
		{
			buff[rsize] = '\0';
			std::cout << "MSG: " << buff << std::endl;
			break ;
		}
	}
	close(fd);
	freeaddrinfo(addr);
	return (0);
}
