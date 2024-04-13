#ifndef SERVER_HPP
# define SERVER_HPP
# include <list>

class Server
{
	private:
		int			_socket_fd;
		addrinfo	_addr_hints;
		addrinfo	*_addr_res;

		std::string _domain_name;
		std::string	_port;

		epoll_event	_events[5];
		int			_epfd;
		int			_timeout_ms;

		std::string	_target;

		void start_addrinfo(void);
		void socket_bind(void);
		void run(void);
		void send_message(epoll_event & event);
		void recv_message(epoll_event & event);
		void new_epoll_event(int conn_fd, uint32_t operation);
		static addrinfo get_hints(void);

	public:
		Server(void);
		Server(const Server & rhs);
		~Server(void);

		void start(void);
};

#endif