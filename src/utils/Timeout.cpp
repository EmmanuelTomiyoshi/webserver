#include "Timeout.hpp"

Timeout::Timeout(void) : _id(0)
{
    std::cout << "Timeout created" << std::endl;
}

Timeout::~Timeout(void)
{
}

void Timeout::add(epoll_event *event)
{
    ft::CustomData *event_data = (ft::CustomData *) event->data.ptr;

    event_data->id = _id;
    _id++;
    _events.push_back(event);
}

void Timeout::remove(epoll_event *event)
{
   std::list<epoll_event *>::iterator it = _events.begin();
   ft::CustomData *event_data = (ft::CustomData *) event->data.ptr;
    for (; it != _events.end(); it++)
    {
        ft::CustomData *li = (ft::CustomData *) (*it)->data.ptr;
        if (li->id == event_data->id)
        {
            _events.erase(it);
            return ;
        }
    }
    std::cerr << "Event not found, failed to remove" << std::endl;
}

void Timeout::event_timed_out(epoll_event *event)
{
    ft::CustomData *event_data = (ft::CustomData *) event->data.ptr;

    event_data->type = ft::TIMEOUT;
    epoll_ctl(event_data->epfd, EPOLL_CTL_DEL, event_data->fd, event);
    std::string response = "HTTP/1.1 500 CGI Unavailable\r\nContent-Type: text; plain\r\nContent-Length: 10\r\n\r\n1234567890";
    send(event_data->cgi_fd, response.c_str(), response.size(), MSG_DONTWAIT);
    close(event_data->cgi_fd);
    close(event_data->fd);
    kill(event_data->pid, SIGKILL);
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

void Timeout::show_count(void)
{
    std::cout << "Events count: " << _events.size() << std::endl;
}

#include <stdio.h>
void Timeout::show_pointers(void)
{
    std::list<epoll_event *>::iterator it = _events.begin();
    for (int i = 0; it != _events.end(); it++, i++)
    {
        printf("Event %d: %p\n", i, *it);
    }
}