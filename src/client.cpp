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

void recv_message(int fd)
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
	
	while (1)
	{
		sleep(1);
		recv_message(fd);
		break ;
	}
	close(fd);
	freeaddrinfo(addr);
	return (0);
}
