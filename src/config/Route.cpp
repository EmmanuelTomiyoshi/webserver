#include <webserver.hpp>

Route::Route(void)
{}
Route::~Route(void) {}

t_str Route::Location::get(void) const
{
	return this->_location;
}

void Route::Location::set(t_str value)
{
	this->_location = value;
}