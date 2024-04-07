#ifndef IPRESOLVER_HPP
#define IPRESOLVER_HPP

#include "webserver.hpp"

class IPResolver
{
	struct addrinfo	*_res;
	struct addrinfo	_hints;

	public:
		IPResolver();
		IPResolver(const IPResolver &copy);
		IPResolver &operator=(const IPResolver &copy);
		~IPResolver();

		void printIPAdresses(const std::string &hostname) const;
};

#endif //IPRESOLVER_HPP