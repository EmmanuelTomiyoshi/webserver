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
}

Server::~Server(void)
{
}

void Server::new_connection(void)
{


}

void	Server::run(void)
{
	while (1) {
		int conn_fd = accept(_socket_fd, NULL, NULL);
		if (conn_fd != -1) {
			int epfd = epoll_create1(EPOLL_CLOEXEC);
			epoll_event event;
			event.events = EPOLLIN | EPOLLOUT;
			event.data.fd = conn_fd;
			epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &event);
			int event_count = epoll_wait(epfd, _events, 5, 30000);
			for (int i = 0; i < event_count; i++)
			{
				char msg[] = "yeaaaaaaaaaaah!!!!!!!";
				send(conn_fd, msg, strlen(msg), MSG_DONTWAIT);
				char buff[200];
				int rsize = recv(_events[i].data.fd, buff, 200, MSG_DONTWAIT);
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
}

void Server::start(void)
{
	start_addrinfo();
	socket_bind();
	listen(_socket_fd, 5);
	std::cout << "Server started." << std::endl;
	run();
}