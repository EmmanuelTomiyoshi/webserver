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

		bool is_allowed(std::string method) const;
		void allow(std::string method);
		void allow(std::vector<std::string> & methods);
		void info(void) const;
};

#endif