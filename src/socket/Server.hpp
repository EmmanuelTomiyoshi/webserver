#ifndef SERVER_HPP
# define SERVER_HPP

class Server
{
	private:
		int			_socket_fd;
		addrinfo	_addr_hints;
		addrinfo	*_addr_res;

		std::string _domain_name;
		std::string	_port;

		epoll_event	_events[5];

		void start_addrinfo(void);
		void socket_bind(void);
		void run(void);
		void new_connection(void);
		static addrinfo get_hints(void);

	public:
		Server(void);
		Server(const Server & rhs);
		~Server(void);

		void start(void);
};

#endif