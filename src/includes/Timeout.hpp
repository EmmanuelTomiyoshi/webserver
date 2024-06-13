#ifndef TIMEOUT_HPP
# define TIMEOUT_HPP
# include "base.hpp"
# include "ft.hpp"

class CustomData;

class Timeout {
    private:
        static std::list<CustomData *> _events;

        static void event_timed_out(CustomData *event);

    public:
        static void add(epoll_event *event);
        static void remove(epoll_event *event);
        static void reset_time(epoll_event *event);

        static void verify(void);

        static void show_count(void);
};

#endif
