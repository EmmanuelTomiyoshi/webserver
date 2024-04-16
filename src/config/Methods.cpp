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

void Methods::allow(std::vector<std::string> & methods)
{
	std::vector<std::string>::iterator it = methods.begin();
	while (it != methods.end())
	{
		bool & m = _methods.at(*it);
		m = true;
		it++;
	}
}

void Methods::info(void) const
{
	std::map<std::string, bool>::const_iterator it;
	it = _methods.begin();
	std::cout << "METHODS INFORMATION:\n";
	while (it != _methods.end())
	{
		std::cout << (*it).first << ": " << ((*it).second ? "true\n" : "false\n");
		it++;
	}
}

