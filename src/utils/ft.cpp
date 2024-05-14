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

    // char *read_text(std::ifstream & file)
    // {
    //     if (file.bad())
    //         throw std::runtime_error("fail: read_text: file.bad");

    //     std::streamsize size = ft::get_file_size(file);
    //     char *buff = new char[size];
    //     file.read(buff, size);
    //     return buff;
    // }

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
}
