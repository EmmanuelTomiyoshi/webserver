#ifndef TIMEOUT_HPP
# define TIMEOUT_HPP
# include "base.hpp"
# include "ft.hpp"

class Timeout {
    private:
        int _ms;
        std::list<epoll_event *> _events;

        void event_timed_out(epoll_event *event);

    public:
        Timeout(int ms);
        ~Timeout(void);

        void add(epoll_event *event);
        void remove(epoll_event *event);

        void verify(void);
};

#endif
