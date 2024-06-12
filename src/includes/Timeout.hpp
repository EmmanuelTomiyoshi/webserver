#ifndef TIMEOUT_HPP
# define TIMEOUT_HPP
# include "base.hpp"
# include "ft.hpp"

class Timeout {
    private:
        std::list<epoll_event *> _events;

        size_t _id;

        void event_timed_out(epoll_event *event);

    public:
        Timeout(void);
        ~Timeout(void);

        void add(epoll_event *event);
        void remove(epoll_event *event);

        void verify(void);

        void show_count(void);
        void show_pointers(void);
};

#endif
