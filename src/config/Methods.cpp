#include <webserver.hpp>

std::vector<std::string> Methods::_methods_available;

Methods::Methods(void)
{
	_methods_available.push_back("GET");
    _methods_available.push_back("POST");
    _methods_available.push_back("UPDATE");
    _methods_available.push_back("DELETE");

	std::vector<std::string>::iterator it;

	for (it = _methods_available.begin(); it != _methods_available.end(); it++)
		_methods[*it] = false;
	std::cout << "Methods created\n";
}

Methods::~Methods(void) { }

Methods & Methods::operator=(Methods const & rhs)
{
	if (this != &rhs)
	{
		std::map<std::string, bool>::const_iterator it;
		it = rhs._methods.begin();
		while (it != rhs._methods.end())
		{
			this->_methods.at((*it).first) = (*it).second;
			it++;
		}
	}
	return (*this);
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

