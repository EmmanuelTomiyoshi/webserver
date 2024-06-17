#ifndef SERVER_HPP
# define SERVER_HPP
# include "Configs.hpp"
# include "Config.hpp"
# include "Response.hpp"
# include "ft.hpp"
# include "Timeout.hpp"

class Server
{
	public:
		typedef void (Server::*event_func)(epoll_event&);
	private:
        std::map<ft::EventType, event_func> _event__functions;
		std::list<addrinfo *> _addr_res_list;
		std::list<int> _socket_fds;

		std::string _domain_name;

		epoll_event	_events[20];
		int			_epfd;

		std::string	_target;

		Configs _configs;
		
		Response *_response;

		void run(void);
		void send_message(void);
		void recv_message(epoll_event & event);
		void recv_client_body(epoll_event & event);

		void process_cgi_response(epoll_event & event);
		void process_request(epoll_event & event);

		static addrinfo get_hints(void);
		void setup(void);

		void close_ports(void);

		static addrinfo *try_server_names(Config *config);
		void setup_config(Config & config);

		void send_data_to_client(epoll_event & event);
		void create_new_connection(epoll_event & event);
		void write_to_cgi(epoll_event & event);


	public:
		Server(std::string config_file);
		~Server(void);

		void create_error_event(int fd, std::string code);
		void start(void);
		void stop(void);
		void new_epoll_event(int conn_fd, uint32_t operation, ft::EventType type);
		void new_epoll_event(int conn_fd, uint32_t operation, ft::EventType type, Config *config);
};

#endif