#ifndef REQUEST2_H
# define REQUEST2_H
# include "base.hpp"
# include "ft.hpp"

/*	 MESSAGE FORMAT 

	 request-line   = method SP request-target SP HTTP-version

 */

class Request2
{

	private:
		std::string _info;
		char *_buff;
		char *_body;
		void separate_info(void);
        void extract_request_line(void);
		void extract_headers(void);

		std::string _method;
		std::string _http_version;
		std::string _target;

		std::map<std::string, std::string> _headers;

	public:
		Request2(char *buff);

        void info(void);
};

#endif