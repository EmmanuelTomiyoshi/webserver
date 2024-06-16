#ifndef CUSTOM_DATA_HPP
# define CUSTOM_DATA_HPP
# include "base.hpp"

class Config;
class Timeout;
class Request2;
class Route;

class CustomData
{
    public:
        CustomData(void);
        ~CustomData(void);

        Config *config;
        Request2 *request;
        char *buff;
        time_t start_time;
        time_t duration;
        int fd;
        int cgi_fd;
        int epfd;
        int pid;
        ft::EventType type;
        ssize_t buff_size;
        ssize_t w_count;
        Route *route;
};

#endif