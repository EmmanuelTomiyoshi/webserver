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
	//TODO: verify what memories to free
	// if (_addr_res != NULL)
	// 	freeaddrinfo(_addr_res);
}

void	Server::new_epoll_event(int conn_fd, uint32_t operation, ft::EventType type)
{
	ft::CustomData *event_data = new ft::CustomData;
	event_data->fd = conn_fd;
	event_data->type = type;
	event_data->epfd = _epfd;

	epoll_event *event = new epoll_event;
	event->events = operation;
	event->data.ptr = (void *) event_data;

	epoll_ctl(_epfd, EPOLL_CTL_ADD, conn_fd, event);
}

void	Server::send_message(epoll_event & event)
{
	ssize_t sent = this->_response->send_response(event);
	delete this->_response;
	if (sent <= 0)
		throw std::runtime_error("empty response");
}

void Server::recv_message(epoll_event & event)
{
	ft::CustomData *event_data = (ft::CustomData *) event.data.ptr;
	char *buff = NULL;
	int buff_size = ft::recv_all(event_data->fd, &buff);
	save_request(buff, buff_size);
	if (buff_size <= 0)
		throw std::runtime_error("empty request");
	std::cout << "buff_size: " << buff_size << std::endl;
	this->_response = new Response(
		buff,
		buff_size,
		_configs._fdconfigs.at(event_data->fd),
		_timeout
	);
}

void Server::process_cgi_response(epoll_event & event)
{
	ft::CustomData *event_data = (ft::CustomData *) event.data.ptr;
	char *buff = NULL;
	_timeout.remove(&event);
	epoll_ctl(_epfd, EPOLL_CTL_DEL, event_data->fd, &event);
	int buff_size = ft::read_all(event_data->fd, &buff);

	CGI cgi;
	cgi.process_response(buff, buff_size);
	char *response = cgi.get_response();
	ssize_t response_size = cgi.get_response_size();

	send(event_data->cgi_fd, response, response_size, MSG_DONTWAIT);

	close(event_data->cgi_fd);
	close(event_data->fd);
}

void Server::process_request(epoll_event & event)
{
	try
	{
		recv_message(event);
		send_message(event);
	}
	catch (std::exception & e)
	{
		epoll_ctl(_epfd, EPOLL_CTL_DEL, event.data.fd, &event);
		close(event.data.fd);
		std::cerr << "REQUEST FAILED: " << e.what() << std::endl;
	}
}

void	Server::run(void)
{
	while (1)
	{
		_timeout.verify();
		int nfds = epoll_wait(this->_epfd, this->_events, 20, 0);
		for (int i = 0; i < nfds; i++)
		{
			ft::CustomData *event_data = (ft::CustomData *) _events[i].data.ptr;
			if (event_data->type == ft::SOCK)
			{
				std::cout << "new connection" << std::endl;
				int fd_conn = accept4(event_data->fd, NULL, NULL, SOCK_NONBLOCK);
				//fd_conn is related to socket_fd that is related to a port that is related to a specific config file
				//this is why this relationship works and I get the right config file in the line below
				_configs._fdconfigs[fd_conn] = _configs._fdconfigs[event_data->fd]; 
				new_epoll_event(fd_conn, EPOLLIN | EPOLLET, ft::CONN);
			}
			else if (event_data->type == ft::CONN)
			{
				std::cout << "request processed" << std::endl;
				process_request(_events[i]);
			}
			else if (event_data->type == ft::CGI)
			{
				std::cout << "cgi response processed" << std::endl;
				process_cgi_response(_events[i]);
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

		new_epoll_event(listen_sock, EPOLLIN, ft::SOCK);
	}
}

void Server::start(void)
{
	setup();
	std::cout << "Server started." << std::endl;
	run();
}
