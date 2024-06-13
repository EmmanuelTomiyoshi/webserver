#ifndef MEMORY_HPP
# define MEMORY_HPP
# include "base.hpp"
# include "ft.hpp"

class Memory
{
    private:
        static std::list<epoll_event *> _m_events;
        static std::list<char *> _m_buffers;
    public:
        Memory(void);

        static void add(epoll_event *);
        static void add(char *);

        static void del(epoll_event *);
        static void del(char *);

        static void remove(epoll_event *);
        static void remove(char *);

        static void clear_events(void);
        static void clear_buffers(void);

        static void clear(void);
};

#endif