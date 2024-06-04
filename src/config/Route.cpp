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


/* ---------------- SAVE FILES PATH ----------------- */
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
	std::cout << "UEEE" << std::endl;
	if (file.bad())
		throw std::runtime_error("page not found");
	std::string content;
	std::getline(file, content, '\0');
	file.close();
	return content;
}

std::string Route::get_path(void) const
{
	std::string path = this->_root + this->location.get();
	return path;
}