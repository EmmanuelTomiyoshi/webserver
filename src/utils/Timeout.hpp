#ifndef TIMEOUT_HPP
# define TIMEOUT_HPP
# include "base.hpp"
# include "ft.hpp"

class Timeout {
    private:
        int _ms;
        std::list<ft::TimeoutData *> _events;

    public:
        Timeout(int ms);
        ~Timeout(void);

        void add(ft::TimeoutData *data);
};

#endif