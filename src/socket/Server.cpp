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
	_timeout_ms = 3000;
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
	std::string msg = "HTTP/1.1 200\r\ncontent-type: text/html; charset=utf-8\r\n\r\n" + _target;
	int sent = send(event.data.fd, msg.c_str(), strlen(msg.c_str()) + 1, MSG_DONTWAIT);
	close(event.data.fd);
	epoll_ctl(_epfd, EPOLL_CTL_DEL, event.data.fd, &event);
	if (sent != -1)
		std::cout << "MESSAGE SENT SUCCESSFULY" << std::endl;
	else
		std::cout << "FAILED TO SEND MESSAGE" << std::endl;
	std::cout << "CONNECTION CLOSED" << std::endl;
	
}

void Server::recv_message(epoll_event & event)
{
	if (event.events != EPOLLIN)
		return ;
	char buff[5000];
	int rsize = recv(event.data.fd, buff, 4999, MSG_DONTWAIT);
	if (rsize > 0) //parse, get response, send response
	{
		buff[rsize] = '\0';
		Request http(buff);
		http.info();
		_target = http.get_target();
		event.events = EPOLLOUT;
		epoll_ctl(_epfd, EPOLL_CTL_MOD, event.data.fd, &event);
		std::cout << "OUTPUT EVENT CREATED" << std::endl;
		return ;
	}
	close(event.data.fd);
	epoll_ctl(_epfd, EPOLL_CTL_DEL, event.data.fd, &event);
	std::cout << "CLOSED CONNECTION: no message" << std::endl;
}

void	Server::run(void)
{
	while (1) {
		int conn_fd = accept(_socket_fd, NULL, NULL);
		if (conn_fd != -1)
			new_epoll_event(conn_fd, EPOLLIN);
		int event_count = epoll_wait(_epfd, _events, 5, _timeout_ms);
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