#include "Timeout.hpp"

Timeout::Timeout(int ms) : _ms(ms)
{
    std::cout << "Timeout created" << std::endl;
}

Timeout::~Timeout(void)
{
}

void Timeout::add(ft::TimeoutData *event)
{
    _events.push_back(event);
}