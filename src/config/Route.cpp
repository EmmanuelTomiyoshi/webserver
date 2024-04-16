#include <webserver.hpp>

Route::Route(void)
{
	std::cout << "Route created\n";
}

Route::~Route(void)
{
}

void Route::Location::set(std::string value)
{
	this->_location = value;
}

std::string Route::Location::get(void) const
{
	return this->_location;
}
