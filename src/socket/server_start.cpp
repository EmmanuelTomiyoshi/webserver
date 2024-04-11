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

int new_epoll(int conn_fd)
{
	int epfd = epoll_create1(EPOLL_CLOEXEC);
	epoll_event event;
	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = conn_fd;
	epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &event);
	return epfd;
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

	std::cout << "server started!" << std::endl;
	epoll_event events[5];
	while (1) {
		int conn_fd = accept(fd, NULL, NULL);
		if (conn_fd != -1) {
			std::cout << "connected!!! " << std::endl;
			int epfd = new_epoll(conn_fd);
			int event_count = epoll_wait(epfd, events, 5, 30000);
			std::cout << "epfd: " << epfd << std::endl;
			std::cout << "event_count: " << event_count << std::endl;
			for (int i = 0; i < event_count; i++)
			{
				char msg[] = "yeaaaaaaaaaaah!!!!!!!";
				send(conn_fd, msg, strlen(msg), MSG_DONTWAIT);
				char buff[200];
				int rsize = recv(events[i].data.fd, buff, 200, MSG_DONTWAIT);
				if (rsize > 0)
				{
					buff[rsize] = '\0';
					std::cout << "Event " << i << ": " << buff << std::endl;
				}
				close(epfd);
			}
			close(conn_fd);
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