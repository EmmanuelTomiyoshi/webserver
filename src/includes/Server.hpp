#ifndef SERVER_HPP
# define SERVER_HPP
# include "Configs.hpp"
# include "Config.hpp"
# include "Response.hpp"
# include "ft.hpp"

class Server
{
	private:
		addrinfo	_addr_hints;
		addrinfo	*_addr_res[5];
		std::list<int> _socket_fds;

		std::string _domain_name;

		epoll_event	_events[20];
		int			_epfd;
		int			_timeout_ms;

		std::string	_target;

		Configs _configs;
		
		Response *_response;

		void run(void);
		void send_message(epoll_event & event);
		void recv_message(epoll_event & event);

		void process_cgi_response(epoll_event & event);
		void process_request(epoll_event & event);

		static addrinfo get_hints(void);
		void setup(void);

	public:
		Server(std::string config_file);
		~Server(void);

		void start(void);
		void new_epoll_event(int conn_fd, uint32_t operation, ft::EventType type);
};

#endif