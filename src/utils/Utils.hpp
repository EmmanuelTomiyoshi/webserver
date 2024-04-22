#ifndef UTILS_HPP
#define UTILS_HPP

#include <webserver.hpp>

namespace utils
{
	class Utils
	{
		private:
			std::map<int, std::string> HttpStatusCode;
		public:
			Utils();
			Utils(const Utils &copy);
			Utils &operator=(const Utils &copy);
			~Utils();
	};
}


#endif //UTILS_HPP