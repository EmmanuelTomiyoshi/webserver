#include <webserver.hpp>

addrinfo Server::get_hints(void)
{
	addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	return hints;
}

void	Server::start_addrinfo(void)
{
	getaddrinfo(
		_domain_name.c_str(),
		_port.c_str(),
		&_addr_hints,
		&_addr_res
	);
}

void	Server::socket_bind(void)
{
	bind(
		_socket_fd,
		_addr_res->ai_addr,
		_addr_res->ai_addrlen
	);
}

Server::Server(void) : _addr_res(NULL)
{
	_socket_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	_port = "3005";
	_domain_name = "localhost";
	_addr_hints = get_hints();
	_epfd = epoll_create1(EPOLL_CLOEXEC);
}

Server::~Server(void)
{
	close(_epfd);
	if (_addr_res != NULL)
		freeaddrinfo(_addr_res);
}

void	Server::new_epoll(int conn_fd)
{
	epoll_event event;
	event.events = EPOLLIN | EPOLLOUT;
	event.data.fd = conn_fd;
	epoll_ctl(_epfd, EPOLL_CTL_ADD, conn_fd, &event);
}

void	Server::send_message(int conn_fd, int event_count)
{
	for (int i = 0; i < event_count; i++)
	{
		char msg[] = "yeaaaaaaaaaaah!!!!!!!";
		send(conn_fd, msg, strlen(msg), MSG_WAITALL);
		std::cout << "message sent" << std::endl;
	}
}

void Server::recv_message(int conn_fd, int event_count)
{
	for (int i = 0; i < event_count; i++)
	{
		char buff[100];
		int rsize = recv(conn_fd, buff, 100, MSG_WAITALL);
		buff[rsize] = '\0';
		std::cout << "MESSAGE: " << buff << std::endl;
	}
}


void	Server::run(void)
{
	while (1) {
		int conn_fd = accept(_socket_fd, NULL, NULL);
		if (conn_fd != -1) {
		new_epoll(conn_fd);
		new_epoll(conn_fd);
		int event_count = epoll_wait(_epfd, _events, 5, 30000);
		send_message(conn_fd, event_count);
		recv_message(conn_fd, event_count);
		close(conn_fd);
		}
	}
}

void Server::start(void)
{
	start_addrinfo();
	socket_bind();
	listen(_socket_fd, 5);
	std::cout << "Server started." << std::endl;
	run();
}