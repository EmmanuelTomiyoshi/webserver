#include <webserver.hpp>

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

int server_start(void)
{
	addrinfo	hints = get_hints();
	addrinfo	*addr = NULL;
	// addrinfo	*rp = NULL;

	int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (fd == SOCKET_ERROR)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	int status = getaddrinfo("localhost", "3005", &hints, &addr);
	if (status != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
		close(fd);
		return (EXIT_FAILURE);
	}

	if (bind(fd, addr->ai_addr, addr->ai_addrlen) == -1)
	{
		perror("bind");
		freeaddrinfo(addr);
		close(fd);
		return (EXIT_FAILURE);
	}

	if (listen(fd, 4) == -1)
	{
		perror("listen");
		freeaddrinfo(addr);
		close(fd);
		return (EXIT_FAILURE);
	}

	server_start();
	std::cout << "server started!" << std::endl;
	while (1) {
		int connFd = accept(fd, NULL, NULL);
		if (connFd != -1) {
			std::cout << "connected!!! " << std::endl;
			char buffer[] = "yeaaaaaaaaaaah!!!!!!!";
			send(connFd, buffer, strlen(buffer), MSG_DONTWAIT);
			close(connFd);
			break ;
		}
	}
	// socklen_t socklen;
	// int nfd = accept(fd, addr->ai_addr, &socklen);
	// std::cout << "nfd: " << nfd << std::endl;
	// perror("listen");
	addr_print(addr);
	freeaddrinfo(addr);
	close(fd);
	return (0);
}