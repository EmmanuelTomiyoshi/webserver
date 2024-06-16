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

/* ---------------- CGI_EXTENSIONS ----------------- */
Route::CGI_Extensions::CGI_Extensions(void)
{
}

std::list<std::string> const & Route::CGI_Extensions::get(void) const
{
	return this->_values;
}

void Route::CGI_Extensions::show(void)
{
	std::list<std::string>::iterator it;
	it = this->_values.begin();
	std::cout << "CGI_Extensions: ";
	while (it != this->_values.end())
	{
		std::cout << (*it) << " ";
		it++;
	}
	std::cout << std::endl;
}

void Route::CGI_Extensions::set(std::list<std::string> const & values) 
{
	std::list<std::string>::const_iterator it;
	it = values.begin();
	while (it != values.end())
	{
		this->_values.push_back(*it);
		it++;
	}
}

bool Route::CGI_Extensions::is_allowed(std::string ext) const
{
	return std::find(
		this->_values.begin(), 
		this->_values.end(), 
		ext
	) != this->_values.end();
}

/* ---------------- SAVE FILES PATH ----------------- */
std::string Route::SaveFilesPath::get(void) const
{
	if (this->_root == "/")
		return this->_save_files_path;
	
	return this->_root + this->_save_files_path;
}

void Route::SaveFilesPath::set(std::string path)
{
	this->_save_files_path = path;
}

void Route::SaveFilesPath::set_root(std::string root)
{
	this->_root = root;
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

bool Route::CGI_Route::is_true(void) const
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
	if (root.empty())
		return ;
	this->_root = ft::get_full_path(root);
	this->save_files_path.set_root(this->_root);
}

void Route::set_parent_root(std::string root)
{
	this->_parent_root = ft::get_full_path(root);
	if (_root.empty())
		this->save_files_path.set_root(get_path());
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
	std::cout << "CGI_Route: " << (cgi_route.is_true() ? "true" : "false") << std::endl;
	this->error_pages.show();
	try_files.show();
	cgi_extensions.show();
}

void Route::validate(void)
{
	if (this->_root.empty() && this->_parent_root.empty())
		throw std::runtime_error("no root defined for route '" + this->location.get() + "'");
}
