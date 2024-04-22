#include "IPResolver.hpp"

IPResolver::IPResolver() : _res(NULL)
{
	std::memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_UNSPEC; //suports both IPv4 and IPv6
	_hints.ai_socktype = SOCK_STREAM; //uses TCP
}

IPResolver::IPResolver(const IPResolver &copy) : _res(NULL)
{
	*this = copy;
}

IPResolver::~IPResolver()
{
	if (_res != NULL)
	{
		freeaddrinfo(_res);
	}
}

IPResolver &IPResolver::operator=(const IPResolver &copy)
{
	if (this != &copy)
	{
		this->_res = copy._res;
		this->_hints = copy._hints;
	}
	return *this;
}

void IPResolver::printIPAdresses(const std::string &hostname) const
{
	int	status;

	// Resolve the hostname
	if ((status = getaddrinfo(hostname.c_str(), NULL, &_hints, const_cast<struct addrinfo **>(&_res))) != 0)
	{
		std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
		return ;
	}

	std::cout << "IP addresses for " << hostname << ":" << std::endl;

	for (struct addrinfo *p = _res; p != NULL; p = p->ai_next)
	{
		const void *addr;
		std::string ipver_str;

		if (p->ai_family == AF_INET)
		{
			struct sockaddr_in *ipv4 = reinterpret_cast<struct sockaddr_in*>(p->ai_addr);
			addr = &(ipv4->sin_addr);
			ipver_str = "IPv4";
		}
		else
		{
			struct sockaddr_in6 *ipv6 = reinterpret_cast<struct sockaddr_in6*>(p->ai_addr);
			addr = &(ipv6->sin6_addr);
			ipver_str = "IPv6";
		}

		//convert IP to a readable string and print it
		std::ostringstream ipstr_stream;
		const unsigned char *ip_bytes = reinterpret_cast<const unsigned char*>(addr);
		ipstr_stream << static_cast<unsigned int>(ip_bytes[0]) << '.'
		             << static_cast<unsigned int>(ip_bytes[1]) << '.'
                     << static_cast<unsigned int>(ip_bytes[2]) << '.'
                     << static_cast<unsigned int>(ip_bytes[3]);
		std::string ipstr = ipstr_stream.str();

		std::cout << ipver_str << ": " << ipstr << std::endl;
	}
}
