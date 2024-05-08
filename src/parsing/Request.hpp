#ifndef REQUEST_H
# define REQUEST_H
# include "base.hpp"

/*	 MESSAGE FORMAT 

	 request-line   = method SP request-target SP HTTP-version

 */

class Request
{

	private:
		std::string _message;
		std::string _request_line;
		std::string _method;
		std::string _target;
		std::string _version;
		void extract_request_line(void);
		void extract_info(void);

	public:
		Request(void);
		Request(std::string message);
		Request(Request const & rhs);
		~Request(void);

		std::string get_message(void) const;
		std::string get_request_line(void) const;
		std::string get_method(void) const;
		std::string get_target(void) const;
		std::string get_version(void) const;
		void init(std::string message);
		void info(void) const;
		void hello(void);
};

#endif