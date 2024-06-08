#include "Route.hpp"

Route::Route(void)
{
}

Route::~Route(void)
{
}

Route & Route::operator=(Route const & rhs)
{
	this->location.set(rhs.location.get());
	this->methods = rhs.methods;
	return (*this);
}

/* ---------------- LOCATION ----------------- */
void Route::Location::set(std::string value)
{
	this->_location = value;
}

std::string Route::Location::get(void) const
{
	return this->_location;
}

/* ---------------- TRY FILES ----------------- */
Route::TryFiles::TryFiles(void)
{
	this->_it = this->_try_files.begin();
}

std::list<std::string> const & Route::TryFiles::get(void) const
{
	return this->_try_files;
}

void Route::TryFiles::show(void)
{
	std::list<std::string>::iterator it;
	it = this->_try_files.begin();
	std::cout << "Try Files: ";
	while (it != this->_try_files.end())
	{
		std::cout << (*it) << " ";
		it++;
	}
	std::cout << std::endl;
}


std::string Route::TryFiles::next(void)
{
	if (this->_it == this->_try_files.end())	
		return "";
	std::string file = *(this->_it);
	this->_it++;
	return file;
}

void Route::TryFiles::set(std::list<std::string> const & files) 
{
	std::list<std::string>::const_iterator it;
	it = files.begin();
	while (it != files.end())
	{
		this->_try_files.push_back(*it);
		it++;
	}
	this->_it = this->_try_files.begin();
}


/* ---------------- SAVE FILES PATH ----------------- */
std::string Route::SaveFilesPath::get(void) const
{
	return this->_save_files_path;
}

void Route::SaveFilesPath::set(std::string path)
{
	this->_save_files_path = path;
}


/* ---------------- AUTOINDEX ----------------- */
Route::Autoindex::Autoindex(void) : _on(false)
{
}

bool Route::Autoindex::get(void) const
{
	return this->_on;
}

void Route::Autoindex::set(bool value)
{
	this->_on = value;
}

void Route::Autoindex::set(std::string value)
{
	this->_on = (value == "on");
}

/* ---------------- CGI_ROUTE ----------------- */
Route::CGI_Route::CGI_Route(void) : _value(false)
{
}

bool Route::CGI_Route::get(void) const
{
	return this->_value;
}

void Route::CGI_Route::set(bool value)
{
	this->_value = value;
}

void Route::CGI_Route::set(std::string value)
{
	this->_value = (value == "true");
}

/* RETURN Or REDIRECT */
void Route::Return::set(std::string value)
{
	this->_value = value;
}

std::string Route::Return::get(void) const
{
	return this->_value;
}

void Route::set_root(std::string root)
{
	this->_root = root;
}

void Route::set_parent_root(std::string root)
{
	this->_parent_root = root;
}

std::string Route::get_root(void) const
{
	return this->_root;
}

std::string Route::get_page(void)
{
	std::string path;
	path = this->_root + this->location.get();

	std::ifstream file;
	std::list<std::string> const & file_names = this->try_files.get();
	std::list<std::string>::const_iterator it;
	it = file_names.begin();
	while (it != file_names.end())
	{
		path = path + (*it);
		file.open(path.c_str());
		if (file.good())
			break ;
		it++;
	}
	if (file.bad())
		throw std::runtime_error("page not found");
	std::string content;
	std::getline(file, content, '\0');
	file.close();
	return content;
}

std::string Route::get_path(void) const
{
	if (_root.empty() == false)
		return _root;

	if (location.get() == "/")
		return _parent_root;
	
	return this->_parent_root + this->location.get();
}

void Route::show(void)
{
	std::cout << "\n--------- ROUTE ---------" << std::endl;
	std::cout << "Root: " << _root << std::endl;
	std::cout << "Location: " << location.get() << std::endl;
	std::cout << "SaveFilesPath: " << save_files_path.get() << std::endl;
	std::cout << "Autoindex: " << (autoindex.get() ? "on" : "off") << std::endl;
	std::cout << "Redirect: " << redirect.get() << std::endl;
	std::cout << "Path: " << get_path() << std::endl;
	std::cout << "CGI_Route: " << (cgi_route.get() ? "true" : "false") << std::endl;
	try_files.show();
}
