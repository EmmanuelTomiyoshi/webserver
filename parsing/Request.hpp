#ifndef REQUEST_H
# define REQUEST_H
# include <string>
# include <sstream>

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
		static std::string extract_request_line(std::string & message);

	public:
		Request(void);
		Request(std::string message);
		Request(Request const & rhs);
		~Request(void);

		std::string get_message(void) const;
		std::string get_request_line(void) const;
		void hello(void);
};

#endif