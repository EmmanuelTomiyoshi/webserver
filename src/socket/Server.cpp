#include "Server.hpp"
#include "CustomData.hpp"

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
	//TODO: verify what memories to free
	// if (_addr_res != NULL)
	// 	freeaddrinfo(_addr_res);
}

void	Server::new_epoll_event(int conn_fd, uint32_t operation, ft::EventType type)
{
	CustomData *event_data = new CustomData;
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
	CustomData *event_data = new CustomData;
	event_data->fd = conn_fd;
	event_data->type = type;
	event_data->epfd = _epfd;
	event_data->timeout = &_timeout;
	event_data->config = config;
	event_data->request = NULL;

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
	if is first time: initialize request

	else: if is body is incomplete, keep reading

	request.add_more_body();

 */

void Server::recv_message(epoll_event & event)
{
	CustomData *event_data = (CustomData *) event.data.ptr;

	char *buff = NULL;
	ssize_t buff_size = ft::recv_all(event_data->fd, &buff);
	std::cout << "data received: " << buff_size << std::endl;
	save_request(buff, buff_size);

	if (buff_size <= 0)
	{
		epoll_ctl(
			event_data->epfd,
			EPOLL_CTL_DEL, event_data->fd,
			&event
		);
		close(event_data->fd);
		return ;
	}

	if (event_data->request == NULL)
	{
		event_data->request = new Request2;
		event_data->request->init(buff, buff_size, event_data->config);
	}
	else if (event_data->request->is_body_complete() == false)
	{
		event_data->request->add_more_body(buff, buff_size);
	}

	event_data->request->debug();

	if (event_data->request->is_body_complete() || event_data->request->is_error())
	{
		event_data->request->info();
		Response response(&event);
		response.send_response();
	}
}

void Server::recv_client_body(epoll_event & event)
{
	(void) event;
}

void Server::process_cgi_response(epoll_event & event)
{
	CustomData *event_data = (CustomData *) event.data.ptr;
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
			CustomData *event_data = (CustomData *) _events[i].data.ptr;
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

		_addr_res_list.push_back(addr_res);

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
	_configs.init();
	_configs.show();
	setup();
	std::cout << "Server started." << std::endl;
	run();
}

void Server::stop(void)
{
	std::cout << "\nstopping server..." << std::endl;
	std::cout << "_epfd: " << _epfd << std::endl;
	close(_epfd);
	close_ports();
}

void Server::close_ports(void)
{
	std::list<int>::iterator it = this->_socket_fds.begin();
	for (; it != _socket_fds.end(); it++)
	{
		std::cout << "closing " << *it << std::endl;
		close(*it);
	}

	std::list<addrinfo *>::iterator it2 = _addr_res_list.begin();
	for (; it2 != _addr_res_list.end(); it2++)
	{
		freeaddrinfo(*it2);
	}
}
