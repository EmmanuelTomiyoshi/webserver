#ifndef ERROR_PAGES_HPP
# define ERROR_PAGES_HPP
# include "base.hpp"

class ErrorPages 
{
    private:
        std::map<std::string, std::string> _pages;
    public:
        void add(std::string code, std::string path);
        std::string get(std::string code);
        void show(void);
};

#endif