#ifndef FT_HPP
# define FT_HPP
#include "base.hpp"

namespace ft {
    bool number_is_in(int value, int *arr, int size);

    //returns all the content in the file
    std::string read_file(std::ifstream & file);

    //has only digits
    bool has_only_digits(std::string str);

}

#endif