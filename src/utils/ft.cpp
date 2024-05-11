#include "ft.hpp"

namespace ft {
    bool number_is_in(int value, int *arr, int size)
    {
        for (int i = 0; i < size; i++)
        {
            if (arr[i] == value)
                return true;
        }
        return false;
    }

    std::string read_file(std::ifstream & file)
    {
        if (file.bad())
            return "";
        
        std::string content;
        while (file.eof() == false)
        {
            std::string aux;
            std::getline(file, aux);
            content += aux;
        }

        return content;
    }

    bool has_only_digits(std::string str)
    {
        return str.find_first_not_of("0123456789") == std::string::npos;
    }
}
