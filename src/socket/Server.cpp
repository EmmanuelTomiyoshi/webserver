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
	event_data->timeout = &_timeout;

	epoll_event *event = new epoll_event;
	event->events = operation;
	event->data.ptr = (void *) event_data;

	epoll_ctl(_epfd, EPOLL_CTL_ADD, conn_fd, event);
}

void	Server::new_epoll_event(int conn_fd, uint32_t operation, ft::EventType type, Config *config)
{
	ft::CustomData *event_data = new ft::CustomData;
	event_data->fd = conn_fd;
	event_data->type = type;
	event_data->epfd = _epfd;
	event_data->timeout = &_timeout;
	event_data->config = config;

	epoll_event *event = new epoll_event;
	event->events = operation;
	event->data.ptr = (void *) event_data;

	epoll_ctl(_epfd, EPOLL_CTL_ADD, conn_fd, event);
}

void	Server::send_message(void)
{
	ssize_t sent = this->_response->send_response();
	delete this->_response;
	if (sent <= 0)
		throw std::runtime_error(HTTP_BAD_REQUEST);
}

/* 
	alocar memoria pro body inteiro

	armazenar a parte que ja recebeu

	terminar de receber o restante

 */

void Server::recv_message(epoll_event & event)
{
	ft::CustomData *event_data = (ft::CustomData *) event.data.ptr;

	char *buff = NULL;
	int buff_size = ft::recv_all(event_data->fd, &buff);
	Request2 *request = new Request2;
	request->init_info(buff, buff_size);
	request->debug();
	event_data->request = request;
	if (request->is_body_complete() == false)
	{
		close_ports();
		exit(0);
	}
	// Response response(&event);
	// response.send_response();
	// save_request(buff, buff_size);
}

void Server::recv_client_body(epoll_event & event)
{
	(void) event;
}


void Server::process_cgi_response(epoll_event & event)
{
	ft::CustomData *event_data = (ft::CustomData *) event.data.ptr;
	char *buff = NULL;
	_timeout.remove(&event);
	epoll_ctl(_epfd, EPOLL_CTL_DEL, event_data->fd, &event);
	int buff_size = ft::read_all(event_data->fd, &buff);

	try
	{
		CGI cgi;
		cgi.process_response(buff, buff_size);
		char *response = cgi.get_response();
		ssize_t response_size = cgi.get_response_size();

		send(event_data->cgi_fd, response, response_size, MSG_DONTWAIT);

		close(event_data->cgi_fd);
		close(event_data->fd);
	}
	catch (std::exception & e)
	{
		Response response;
		response.process_error(e.what());
		send(
			event_data->cgi_fd,
			response.get_response(),
			response.get_response_size(),
			MSG_DONTWAIT
		);
	}

}

void Server::process_request(epoll_event & event)
{
	try
	{
		recv_message(event);
		// send_message();
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
				new_epoll_event(
					fd_conn,
					EPOLLIN | EPOLLET,
					ft::CONN,
					_configs._fdconfigs[event_data->fd]
				);
			}
			else if (event_data->type == ft::CONN)
			{
				std::cout << "connection event triggered" << std::endl;
				process_request(_events[i]);
			}
			else if (event_data->type == ft::CGI_R)
			{
				std::cout << "cgi read event triggered" << std::endl;
				process_cgi_response(_events[i]);
			}
			else if (event_data->type == ft::CGI_W)
			{
				std::cout << "cgi write event triggered" << std::endl;
				CGI cgi;
				cgi.write_to_cgi(&_events[i]);
			}
			else if (event_data->type == ft::CLIENT_BODY)
			{
				std::cout << "client body event triggered" << std::endl;
				recv_client_body(_events[i]);
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

void Server::close_ports(void)
{
	std::list<int>::iterator it = this->_socket_fds.begin();
	for (; it != _socket_fds.end(); it++)
		close(*it);
}
