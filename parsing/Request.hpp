#ifndef REQUEST_H
# define REQUEST_H

class Request
{
	public:
		Request(void);
		Request(Request const & rhs);
		~Request(void);

		void hello(void);
};

#endif