#include "ErrorPages.hpp"

/* --------------- ERROR_PAGES --------------- */
void ErrorPages::add(std::string code, std::string path)
{
	if (code.empty() || path.empty())
		return ;
	if (code.size() != 3)
		return ;
	if (path[0] != '.' && path[0] != '/')
		return ;
	for (size_t i = 0; i < code.size(); i++)
	{
		if (std::isdigit(code[i]) == false)
			return ;
	}
	this->_pages[code] = path;
}

std::string ErrorPages::get(std::string code)
{
	try
	{
		return this->_pages.at(code);
	}
	catch(const std::exception& e)
	{
		return "";
	}
	
}

void ErrorPages::show(void)
{
	std::map<std::string, std::string>::const_iterator it;
	it = this->_pages.begin();
	std::cout << "---------- Error Pages ----------" << std::endl;
	for (; it != this->_pages.end(); it++)
	{
		std::cout << it->first << ": " << it->second << std::endl;
	}
}
