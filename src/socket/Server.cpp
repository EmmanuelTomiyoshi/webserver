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

void	Server::new_epoll_event(int conn_fd, uint32_t operation)
{
	epoll_event event;
	event.events = operation;
	event.data.fd = conn_fd;
	epoll_ctl(_epfd, EPOLL_CTL_ADD, conn_fd, &event);
}

void	Server::send_message(epoll_event & event)
{
	if (event.events != EPOLLOUT)
		return ;
	char msg[] = "yeaaaaaaaaaaah!!!!!!!";
	send(event.data.fd, msg, strlen(msg), MSG_WAITALL);
	std::cout << "MESSAGE SENT" << std::endl;
	epoll_ctl(_epfd, EPOLL_CTL_DEL, event.data.fd, &event);
	close(event.data.fd);
	std::cout << "CONNECTION CLOSED" << std::endl;
}

void Server::recv_message(epoll_event & event)
{
	if (event.events != EPOLLIN)
		return ;
	char buff[100];
	int rsize = recv(event.data.fd, buff, 100, MSG_WAITALL);
	buff[rsize] = '\0';
	Request http(buff);
	std::cout << "REQUEST LINE: " << http.get_request_line() << std::endl;
	std::cout << "METHOD: " << http.get_method() << std::endl;
	epoll_ctl(_epfd, EPOLL_CTL_DEL, event.data.fd, &event);
	new_epoll_event(event.data.fd, EPOLLOUT);
	std::cout << "OUTPUT EVENT CREATED" << std::endl;
}

void	Server::run(void)
{
	while (1) {
		int conn_fd = accept(_socket_fd, NULL, NULL);
		if (conn_fd != -1)
			new_epoll_event(conn_fd, EPOLLIN);
		int event_count = epoll_wait(_epfd, _events, 5, 5);
		for (int i = 0; i < event_count; i++)
		{
			recv_message(_events[i]);
			send_message(_events[i]);
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