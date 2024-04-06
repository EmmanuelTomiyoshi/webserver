#include "Request.hpp"
#include <iostream>
Request::Request(void) {}



Request::Request(std::string message) {
	this->_message = message;
	this->extract_request_line();
}

std::string Request::get_message(void) const {
	return this->_message;
}

std::string Request::get_request_line(void) const {
	return this->_request_line;
}
//LF = 10 = Line Feed
//CR = 13 = Carriage Return

void Request::extract_request_line(void) {
	std::stringstream	ss(this->_message);
	char	buffer[200];
	ss.getline(buffer, 199);
	this->_request_line = std::string(buffer);
}

Request::~Request(void) {}

void Request::hello(void) {
	std::cout << "Hello my friends!!!" << std::endl;
}
