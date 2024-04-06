#ifndef REQUEST_H
# define REQUEST_H
# include <string>

/*	 MESSAGE FORMAT 

	 request-line   = method SP request-target SP HTTP-version

 */

class Request
{

	private:
		std::string _message;
		std::string extract_request_line();

	public:
		Request(void);
		Request(std::string message);
		Request(Request const & rhs);
		~Request(void);

		std::string get_message(void) const;
		void hello(void);
};

#endif