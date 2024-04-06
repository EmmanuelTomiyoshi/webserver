#include "Request.hpp"
#include <iostream>
Request::Request(void) {}



Request::Request(std::string message) {
	this->_message = message;
}

std::string Request::get_message(void) const {
	return this->_message;
}

Request::~Request(void) {}

void Request::hello(void) {
	std::cout << "Hello my friends!!!" << std::endl;
}
