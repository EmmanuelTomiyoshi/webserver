#include <webserver.hpp>

std::vector<std::string> Methods::_methods_available = {
	"GET",
	"POST",
	"UPDATE",
	"DELETE"
};

Methods::Methods(void)
{
	std::vector<std::string>::iterator it;

	for (it = _methods_available.begin(); it != _methods_available.end(); it++)
		_methods[*it] = false;
	std::cout << "Methods created\n";
}

Methods::~Methods(void)
{
}

bool Methods::is_allowed(std::string method) const
{
	return this->_methods.at(method);
}

void Methods::allow(std::string method)
{
	bool & m = this->_methods.at(method);
	m = true;
}