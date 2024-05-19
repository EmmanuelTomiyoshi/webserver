#include "Request2.hpp"

Request2::Request2(char *buff) : _buff(buff), _body(NULL)
{
	separate_info();
	std::cout << "----- Request2 INFO----\n" << _info << std::endl;
}

void Request2::separate_info(void)
{
	_body = ft::get_body_position(_buff);
	size_t size_info = _body - _buff;
	char *info = new char[size_info + 1];
	std::memmove(info, _buff, size_info);
	info[size_info] = '\0';
	_info = info;
	delete [] info;
}
