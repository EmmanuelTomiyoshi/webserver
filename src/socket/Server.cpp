#include "Server.hpp"

addrinfo Server::get_hints(void)
{
	addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	return hints;
}

Server::Server(std::string config_file) : _configs(config_file)
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
	ssize_t sent = this->_response->send_response(event.data.fd);
	epoll_ctl(_epfd, EPOLL_CTL_DEL, event.data.fd, &event);
	close(event.data.fd);
	delete this->_response;
	if (sent == -1)
		std::cout << "FAILED TO SEND MESSAGE" << std::endl;
	else
		std::cout << "MESSAGE SENT SUCCESSFULY" << std::endl;
}

void Server::recv_message(epoll_event & event)
{
	char buff[5000];
	int rsize = recv(event.data.fd, buff, 4999, MSG_WAITALL);
	if (rsize < 0) //parse, get response, send response
	{
		close(event.data.fd);
		epoll_ctl(_epfd, EPOLL_CTL_DEL, event.data.fd, &event);
		std::cout << "CLOSED CONNECTION ERROR: no message" << std::endl;
		return ;
	}
	buff[rsize] = '\0';
	this->_response = new Response(
		buff,
		_configs._fdconfigs.at(event.data.fd)
	);
}

void	Server::run(void)
{
	while (1)
	{
		int nfds = epoll_wait(this->_epfd, this->_events, 20, -1);
		for (int i = 0; i < nfds; i++)
		{
			if (std::find(
				_socket_fds.begin(),
				_socket_fds.end(), 
				_events[i].data.fd) != _socket_fds.end())
			{
				std::cout << "new connection" << std::endl;
				int fd_conn = accept4(_events[i].data.fd, NULL, NULL, SOCK_NONBLOCK);
				//fd_conn is related to socket_fd that is related to a port that is related to a specific config file
				//this is why this relationship works and I get the right config file in the line below
				_configs._fdconfigs[fd_conn] = _configs._fdconfigs[_events[i].data.fd]; 
				new_epoll_event(fd_conn, EPOLLIN | EPOLLET);
			}
			else
			{
				recv_message(_events[i]);
				send_message(_events[i]);
			}
		}
	}
}

void Server::setup(void)
{
	_addr_hints = get_hints();
	_epfd = epoll_create1(0);
	if (_epfd == -1)
		throw std::runtime_error("error creating epoll");
	_timeout_ms = 0;
	addrinfo *addr_res;

	std::list<Config>::iterator it;
	it = _configs.get().begin();

	for (; it != _configs.get().end(); it++)
	{
		Config & config = (*it);
		// config.routes.get("/").get_page();
		_domain_name = config.host.get();

		getaddrinfo(
			_domain_name.c_str(),
			config.port.get().c_str(),
			&_addr_hints,
			&addr_res
		);

		const int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
		_socket_fds.push_back(listen_sock);
		_configs._fdconfigs[listen_sock] = &(*it); //insert the current config address in fdconfigs map

		bind(
			listen_sock,
			addr_res->ai_addr,
			addr_res->ai_addrlen
		);

		listen(listen_sock, 20);

		new_epoll_event(listen_sock, EPOLLIN);
	}
}

void Server::start(void)
{
	setup();
	std::cout << "Server started." << std::endl;
	run();
}
