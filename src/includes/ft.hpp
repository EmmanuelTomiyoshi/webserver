#ifndef FT_HPP
# define FT_HPP
#include "base.hpp"

class Config;
class Request2;
class Timeout;

namespace ft {

    enum EventType
    {
        SOCK,
        CONN,
        CGI_R,
        CGI_W,
        TIMEOUT,
        TRASH,
        CLIENT_BODY,
    };

    typedef struct CustomData
    {
        int fd;
        int cgi_fd;
        int epfd;
        Timeout *timeout;
        time_t start_time;
        time_t duration;
        int pid;
        EventType type;
        size_t id;
        char *buff;
        ssize_t buff_size;
        ssize_t w_count;
        Config *config;
        Request2 *request;
    } CustomData;

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

    ssize_t read_all(int fd, char **buff);
    bool file_exists(std::string filename);

    //receives a relative path and returns a full path
    std::string get_full_path(std::string relative);
}

#endif