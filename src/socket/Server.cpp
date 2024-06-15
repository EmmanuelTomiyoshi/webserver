#include "Server.hpp"
#include "CustomData.hpp"
#include "Memory.hpp"

addrinfo Server::get_hints(void)
{
	addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	return hints;
}

Server::Server(std::string config_file) : _configs(config_file)
{
	_event__functions[ft::SOCK] = &Server::create_new_connection;
    _event__functions[ft::CONN] = &Server::process_request;
    _event__functions[ft::CGI_R] = &Server::process_cgi_response;
    _event__functions[ft::CGI_W] = &Server::write_to_cgi;
	_event__functions[ft::CLIENT_BODY] = &Server::recv_client_body;
	_event__functions[ft::RESPONSE] = &Server::send_data_to_client;
}

Server::~Server(void)
{
	//TODO: verify what memories to free
	// if (_addr_res != NULL)
	// 	freeaddrinfo(_addr_res);
}

void	Server::new_epoll_event(int conn_fd, uint32_t operation, ft::EventType type, Config *config)
{
	CustomData *event_data = new CustomData;
	event_data->fd = conn_fd;
	event_data->type = type;
	event_data->epfd = _epfd;
	event_data->config = config;
	event_data->request = NULL;

	epoll_event event;
	event.events = operation;
	event.data.ptr = (void *) event_data;

	Memory::add(&event);

	epoll_ctl(_epfd, EPOLL_CTL_ADD, conn_fd, &event);
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

void	Server::new_epoll_event(int conn_fd, uint32_t operation, ft::EventType type)
{
	CustomData *event_data = new CustomData;
	event_data->fd = conn_fd;
	event_data->type = type;
	event_data->epfd = _epfd;
	event_data->request = NULL;

	epoll_event event;
	event.events = operation;
	event.data.ptr = (void *) event_data;

	Memory::add(&event);

	epoll_ctl(_epfd, EPOLL_CTL_ADD, conn_fd, &event);
}

void Server::recv_message(epoll_event & event)
{
	CustomData *event_data = (CustomData *) event.data.ptr;

	const ssize_t buff_size = 40000;
	char buff[buff_size];
	ssize_t bytes = recv(event_data->fd, buff, buff_size, MSG_DONTWAIT);
	std::cout << "data received: " << bytes << std::endl;

	if (bytes <= 0)
	{
		epoll_ctl(
			event_data->epfd,
			EPOLL_CTL_DEL,
			event_data->fd,
			NULL
		);
		close(event_data->fd);
		Memory::del(&event);
		return ;
	}

	char *data = new char[bytes];
	std::memmove(data, buff, bytes);

	if (event_data->request == NULL)
	{
		event_data->request = new Request2;
		event_data->request->init(data, bytes, event_data->config);
	}
	else if (event_data->request->is_body_complete() == false)
	{
		event_data->request->add_more_body(data, bytes);
		delete [] data;
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
	Timeout::remove(&event);
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
		delete [] response;
		if (buff)
			free(buff);
		Memory::del(&event);
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
		Memory::del(&event);
		std::cerr << "REQUEST FAILED: " << e.what() << std::endl;
	}
}

void Server::send_data_to_client(epoll_event & event)
{
	CustomData *data = (CustomData *) event.data.ptr;
	ssize_t bytes = 0;
	if (data->w_count < data->buff_size)
	{
		bytes = send(
			data->fd,
			data->buff + data->w_count, 
			data->buff_size - data->w_count,
			MSG_DONTWAIT
		);

		std::cout << bytes << " sent." << std::endl;

		data->w_count += bytes;
	}
	
	if (bytes <= 0 || data->w_count >= data->buff_size)
	{
		epoll_ctl(data->epfd, EPOLL_CTL_DEL, data->fd, NULL);
		close(data->fd);
		delete [] data->buff;
		Memory::remove(data);
		return ;
	}
}

void Server::create_new_connection(epoll_event & event)
{
	CustomData *event_data = (CustomData *) event.data.ptr;
	std::cout << "new connection" << std::endl;
	int fd_conn = accept4(event_data->fd, NULL, NULL, SOCK_NONBLOCK);
	//fd_conn is related to socket_fd that is related to a port that is related to a specific config file
	//this is why this relationship works and I get the right config file in the line below
	_configs._fdconfigs[fd_conn] = _configs._fdconfigs[event_data->fd];
	new_epoll_event(
		fd_conn,
		EPOLLIN,
		ft::CONN,
		_configs._fdconfigs[event_data->fd]
	);
}

void Server::write_to_cgi(epoll_event & event)
{
	std::cout << "cgi write event triggered" << std::endl;
	CGI cgi;
	cgi.write_to_cgi(event);
}

void	Server::run(void)
{
	while (1)
	{
		Timeout::verify();
		int nfds = epoll_wait(this->_epfd, this->_events, 20, 0);
		for (int i = 0; i < nfds; i++)
		{
			CustomData *event_data = (CustomData *) _events[i].data.ptr;
			try
			{
				Server::event_func func = _event__functions.at(event_data->type);
				(this->*func)(_events[i]);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
			
		}
	}
}

addrinfo *Server::try_server_names(Config *config)
{
	std::list<std::string>::const_iterator it;
	it = config->server_names.get().begin();

	addrinfo hints = get_hints();
	addrinfo *res;
	for (; it != config->server_names.get().end(); it++)
	{
		int status = getaddrinfo(
			it->c_str(),
			config->port.get().c_str(),
			&hints,
			&res
		);
		if (status == 0)
			return res;
	}
	return NULL;
}

void Server::setup_config(Config & config)
{
	std::list<std::string>::const_iterator it;
	it = config.server_names.get().begin();

	addrinfo addr_hints = get_hints();
	addrinfo *addr_res;
	for (; it != config.server_names.get().end(); it++)
	{
		int status = getaddrinfo(
			it->c_str(),
			config.port.get().c_str(),
			&addr_hints,
			&addr_res
		);
		if (status != 0)
		{
			continue ;
		}

		const int listen_sock = socket(AF_INET, SOCK_STREAM, 0);

		if (bind(listen_sock, addr_res->ai_addr, addr_res->ai_addrlen) != 0)
		{
			freeaddrinfo(addr_res);
			continue ;
		}

		if (listen(listen_sock, 20) != 0)
		{
			freeaddrinfo(addr_res);
			continue ;
		}

		std::cout << "successfuly listening to " << it->c_str() << ":" << config.port.get() << std::endl;

		_addr_res_list.push_back(addr_res);
		_socket_fds.push_back(listen_sock);
		_configs._fdconfigs[listen_sock] = &config; //insert the current config address in fdconfigs map
		new_epoll_event(listen_sock, EPOLLIN, ft::SOCK);
		return ;
	}
	throw std::runtime_error("a server wasn't able to be started");
}

void Server::setup(void)
{
	_epfd = epoll_create1(0);
	if (_epfd == -1)
		throw std::runtime_error("error creating epoll");

	std::list<Config>::iterator it;
	it = _configs.get().begin();
	size_t count = 0;
	for (; it != _configs.get().end(); it++)
	{
		Config & config = (*it);

		try
		{
			setup_config(config);
		}
		catch(const std::exception& e)
		{
			std::cout << "Failed to start a server with the following servernames:\n";
			config.server_names.show();
			std::cout << std::endl;
			count++;
		}
	}
	if (_configs.get().size() == count)
		throw std::runtime_error("no server could be started, make sure to configure it properly");
}

void Server::start(void)
{
	_configs.init();
	setup();
	std::cout << "Server started." << std::endl;
	run();
}

void Server::stop(void)
{
	std::cout << "\nstopping server..." << std::endl;
	close(_epfd);
	close_ports();
}

void Server::close_ports(void)
{
	std::list<int>::iterator it = this->_socket_fds.begin();
	for (; it != _socket_fds.end(); it++)
	{
		close(*it);
	}

	std::list<addrinfo *>::iterator it2 = _addr_res_list.begin();
	for (; it2 != _addr_res_list.end(); it2++)
	{
		freeaddrinfo(*it2);
	}
}
