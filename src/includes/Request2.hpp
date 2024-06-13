#ifndef REQUEST2_H
# define REQUEST2_H
# include "base.hpp"
# include "ft.hpp"
# include "error_codes.hpp"

/*	 MESSAGE FORMAT 

	 request-line   = method SP request-target SP HTTP-version

 */

class Config;

class Request2
{

	private:
		Config *_config;
		std::string _error;

		std::string _info;
		std::string _info_raw;
		char *_buff;
		ssize_t _buff_size;
		char *_body;
		ssize_t _body_size;
		ssize_t _body_bytes;
		void separate_info(void);
        void extract_request_line(void);
		void extract_headers(void);
		void extract_body(void);

		std::string _method;
		std::string _http_version;
		std::string _target;

		std::map<std::string, std::string> _headers;

		void verify_initialization(void) const;

		bool _is_cgi;
		std::string _file;
		std::string _route;
		std::string _query;
		void extract_route(void);
		void extract_route_normal(void);
		void extract_route_delete(void);
		void extract_file(void);
		void extract_file_normal(void);
		void extract_file_delete(void);
		void extract_query(void);
		void validations(void);

	public:
		Request2(void);
		~Request2(void);

		void init(char *buff, ssize_t size, Config *config);
		std::string get_header(std::string key) const;
		std::string get_method(void);
		std::string get_target(void);
		std::string get_version(void);
		char *get_body(void);
		ssize_t get_body_size(void);
        void info(void);
		void debug(void);

		std::string get_file(void);
		std::string get_route(void);
		std::string get_query(void);
		bool is_cgi(void);

		ssize_t body_bytes_remaining(void);
		bool is_body_complete(void);
		void add_more_body(char *buff, ssize_t size);

		bool is_error(void);
		std::string get_error(void);
};

#endif