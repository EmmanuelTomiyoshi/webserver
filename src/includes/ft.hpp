#ifndef FT_HPP
# define FT_HPP
#include "base.hpp"

namespace ft {
    bool number_is_in(int value, int *arr, int size);

    std::streamsize get_file_size(std::ifstream & file);


    //returns all the content in the file
    void *read_binary(std::ifstream & file, std::streamsize size);
    // char *read_text(std::ifstream & file);

    //has only digits
    bool has_only_digits(std::string str);

    std::string int_to_str(size_t num);

    char *cgi(
        char *content_type, 
        char *request_method, 
        char *script_name
    );

    size_t str_to_int(std::string str);

    char *get_body_position(char *data);
    char *get_body_position(char *response, size_t response_size);

    void debug_file(std::string filename, char *buff, size_t size);

    void print_char_array(const char *arr, size_t size);
    void print_char_array(const char *arr);

    ssize_t recv_all(int fd, char **buff);

    ssize_t write_all(int fd, char *buff, ssize_t size);
    void close_pipes(int *pfds1, int *pfds2);
}

#endif