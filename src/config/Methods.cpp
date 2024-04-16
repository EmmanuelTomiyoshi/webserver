#include <webserver.hpp>

t_vec Methods::_methods_available = {
	"GET",
	"POST",
	"UPDATE",
	"DELETE"
};

Methods::Methods(void)
{
	t_vec::iterator it;

	for (it = this->_methods_available.begin(); it != this->_methods_available.end(); it++)
		_methods[*it] = false;
}

Methods::~Methods(void) {}

void Methods::set(t_list values)
{
	t_list::iterator it;

	for (it = values.begin(); it != values.end(); it++)
	{
		try
		{
			bool & allowed = this->_methods.at(*it);
			allowed = true;
		}
		catch (std::exception & e)
		{
			std::cerr << "Invalid method: " << *it << std::endl;
		}
	}
}

bool Methods::allowed(t_str method) const
{
	return this->_methods.at(method); //catch exception??
}
