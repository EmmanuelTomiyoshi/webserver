#include "Request.hpp"
#include <iostream>
Request::Request(void) {}



Request::Request(std::string message) {
	this->_message = message;
	this->extract_request_line();
	this->extract_info();
}

std::string Request::get_message(void) const {
	return this->_message;
}

std::string Request::get_request_line(void) const {
	return this->_request_line;
}

std::string Request::get_method(void) const {
	return this->_method;
}

std::string Request::get_target(void) const {
	return this->_target;
}

std::string Request::get_version(void) const {
	return this->_version;
}

void Request::info(void) const {
	std::cout << "------- INFO -------" << std::endl;
	std::cout << "Method: " << this->_method << std::endl;
	std::cout << "Target: " << this->_target << std::endl;
	std::cout << "Version: " << this->_version << std::endl;
}
//LF = 10 = Line Feed
//CR = 13 = Carriage Return

void Request::extract_request_line(void) {
	std::stringstream	ss(this->_message);
	char	buffer[200];
	ss.getline(buffer, 199);
	this->_request_line = std::string(buffer);
}

void Request::extract_info(void) {
	std::stringstream ss(this->_request_line);

	ss >> this->_method;
	ss >> this->_target;
	ss >> this->_version;
}

Request::~Request(void) {}

void Request::hello(void) {
	std::cout << "Hello my friends!!!" << std::endl;
}
