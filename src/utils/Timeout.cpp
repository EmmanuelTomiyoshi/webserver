#include "Timeout.hpp"

Timeout::Timeout(int ms) : _ms(ms)
{
    std::cout << "Timeout created" << std::endl;
}

Timeout::~Timeout(void)
{
}

void Timeout::add(epoll_event *event)
{
    _events.push_back(event);
}

void Timeout::remove(epoll_event *event)
{
    _events.remove(event);
}

void Timeout::event_timed_out(epoll_event *event)
{
    ft::CustomData *event_data = (ft::CustomData *) event->data.ptr;

    close(event_data->cgi_fd);
    close(event_data->fd);
    kill(event_data->pid, SIGKILL);
    epoll_ctl(event_data->epfd, EPOLL_CTL_DEL, event_data->fd, event);
    remove(event);
    std::cout << "Event timed out" << std::endl;
}

void Timeout::verify(void)
{
    std::list<epoll_event *>::iterator it = _events.begin();
    for (; it != _events.end(); it++)
    {
        ft::CustomData *event_data = (ft::CustomData *)(*it)->data.ptr;
        if (event_data->start_time + event_data->timeout < time(NULL))
        {
            event_timed_out(*it);
            return ;
        }
    }
}
