#ifndef METHODS_HPP
# define METHODS_HPP
#include <webserver.hpp>

class Methods
{
	private:
		static std::vector<std::string> _methods_available;

		std::map<std::string, bool> _methods;

	public:
		Methods(void);
		~Methods(void);
		Methods & operator=(Methods const & rhs);

		bool allowed(std::string method) const;
		void set(std::list<std::string> & methods);
		void show(void) const;
};

#endif