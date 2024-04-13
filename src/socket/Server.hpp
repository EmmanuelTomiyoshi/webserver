#ifndef SERVER_HPP
# define SERVER_HPP
# include <list>

class Server
{
	private:
		addrinfo	_addr_hints;
		addrinfo	*_addr_res[5];
		int			_socket_fds[5];

		std::string _domain_name;

		epoll_event	_events[5];
		int			_epfd;
		int			_timeout_ms;

		std::string	_target;

		void run(void);
		void send_message(epoll_event & event);
		void recv_message(epoll_event & event);
		void new_epoll_event(int conn_fd, uint32_t operation);
		static addrinfo get_hints(void);
		void setup(void);

	public:
		Server(void);
		Server(const Server & rhs);
		~Server(void);

		void start(void);
};

#endif