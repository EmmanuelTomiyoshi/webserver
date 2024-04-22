#ifndef UTILS_HPP
#define UTILS_HPP

#include <webserver.hpp>

namespace utils
{
	class Utils
	{
		private:
			std::map<int, std::string> _HttpStatusCode;

		public:
			Utils();
			Utils(const Utils &copy);
			Utils &operator=(const Utils &copy);
			~Utils();

			bool hasInvalidURICharacters(const std::string &str);

			std::map<int, std::string> getHttpStatusCode() const;
	};

}

#endif //UTILS_HPP