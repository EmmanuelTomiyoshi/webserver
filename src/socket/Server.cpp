#include <webserver.hpp>

addrinfo Server::get_hints(void)
{
	addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	return hints;
}

Server::Server(void)
{
}

Server::~Server(void)
{
	close(_epfd);
	// if (_addr_res != NULL)
	// 	freeaddrinfo(_addr_res);
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
		for (int i = 0; i < 5; i++)
		{
			int conn_fd = accept(_socket_fds[i], NULL, NULL);
			if (conn_fd != -1)
				new_epoll_event(conn_fd, EPOLLIN);
		}
		int event_count = epoll_wait(_epfd, _events, 5, _timeout_ms);
		for (int i = 0; i < event_count; i++)
		{
			recv_message(_events[i]);
			send_message(_events[i]);
		}
	}
}

void Server::setup(void)
{
	_domain_name = "localhost";
	_addr_hints = get_hints();
	_epfd = epoll_create1(EPOLL_CLOEXEC);
	_timeout_ms = 0;

	const char	*ports[5] = {"3004", "3005", "3006", "3007", "3008"};

	for (int i = 0; i < 5; i++)
	{
		getaddrinfo(
			_domain_name.c_str(),
			ports[i],
			&_addr_hints,
			&_addr_res[i]
		);

		_socket_fds[i] = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

		bind(
			_socket_fds[i],
			_addr_res[i]->ai_addr,
			_addr_res[i]->ai_addrlen
		);

		listen(_socket_fds[i], 5);
	}
}

void Server::start(void)
{
	setup();
	std::cout << "Server started." << std::endl;
	run();
}