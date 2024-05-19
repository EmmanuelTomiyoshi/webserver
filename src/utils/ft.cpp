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

    std::streamsize get_file_size(std::ifstream & file)
    {
        file.seekg(0, std::ios::end);
        std::streamsize file_size = file.tellg();
        file.seekg(0, std::ios::beg);
        return file_size;
    }

    void *read_binary(std::ifstream & file, std::streamsize size)
    {
        if (file.bad() || size == 0)
            return NULL;

        char *data = new char[size];
        file.read(data, size);
        return data;
    }

    bool has_only_digits(std::string str)
    {
        return str.find_first_not_of("0123456789") == std::string::npos;
    }

    std::string int_to_str(size_t num)
    {
        std::ostringstream oss;
        oss << num;
        return oss.str();
    }

    size_t str_to_int(std::string str)
    {
        std::stringstream ss(str);
        size_t n;
        ss >> n;
        return n;
    }

    char *get_body_position(char *data)
    {
        while (*data)
        {
            if (std::strncmp(data, "\n\n", 2) == 0)
            {
                return data + 2;
            }
            data++;
        }
        return data;
    }
}
