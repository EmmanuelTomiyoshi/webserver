#ifndef REQUEST2_H
# define REQUEST2_H
# include "base.hpp"
# include "ft.hpp"
# include "error_codes.hpp"

/*	 MESSAGE FORMAT 

	 request-line   = method SP request-target SP HTTP-version

 */

class Request2
{

	private:
		std::string _info;
		char *_buff;
		size_t _buff_size;
		char *_body;
		size_t _body_size;
		void separate_info(void);
        void extract_request_line(void);
		void extract_headers(void);
		void extract_body_size(void);

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
		void extract_file(void);
		void extract_query(void);

	public:
		Request2(void);

		void init(char *buff, ssize_t size);
		std::string get_header(std::string key) const;
		std::string get_method(void);
		std::string get_target(void);
		std::string get_version(void);
		char *get_body(void);
		size_t get_body_size(void);
        void info(void);

		std::string get_file(void);
		std::string get_route(void);
		std::string get_query(void);
		bool is_cgi(void);
};

#endif