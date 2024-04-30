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

bool Methods::allow(std::string method) const
{
	try
	{
		this->_methods.at(method);
		return true;
	}
	catch (std::exception & e)
	{
		return false;
	}
}

void Methods::set(std::list<std::string> & methods)
{
	std::list<std::string>::iterator it = methods.begin();
	while (it != methods.end())
	{
		_methods.at(*it) = true;
		it++;
	}
}

void Methods::show(void) const
{
	std::map<std::string, bool>::const_iterator it;
	it = _methods.begin();
	std::cout << "METHODS: ";
	while (it != _methods.end())
	{
		std::cout << ((*it).second ? (*it).first : "") << " ";
		it++;
	}
	std::cout << std::endl;
}

