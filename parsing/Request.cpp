#include "Request.hpp"
#include <iostream>
Request::Request(void) {}



Request::Request(std::string message) {
	this->_message = message;
	this->_request_line = extract_request_line(message);
}

std::string Request::get_message(void) const {
	return this->_message;
}

std::string Request::get_request_line(void) const {
	return this->_request_line;
}
//LF = 10 = Line Feed
//CR = 13 = Carriage Return

std::string Request::extract_request_line(std::string & message) {
	std::stringstream	ss(message);
	char	buffer[200];
	ss.getline(buffer, 199);
	return std::string(buffer);
}

Request::~Request(void) {}

void Request::hello(void) {
	std::cout << "Hello my friends!!!" << std::endl;
}
