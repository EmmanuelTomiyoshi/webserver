#include "Timeout.hpp"
# include "Response.hpp"
#include "CustomData.hpp"

Timeout::Timeout(void) : _id(0)
{
    std::cout << "Timeout created" << std::endl;
}

Timeout::~Timeout(void)
{
}

void Timeout::add(epoll_event *event)
{
    CustomData *event_data = (CustomData *) event->data.ptr;

    event_data->id = _id;
    _id++;
    _events.push_back(event);
}

void Timeout::remove(epoll_event *event)
{
   std::list<epoll_event *>::iterator it = _events.begin();
   CustomData *event_data = (CustomData *) event->data.ptr;
    for (; it != _events.end(); it++)
    {
        CustomData *li = (CustomData *) (*it)->data.ptr;
        if (li->id == event_data->id)
        {
            _events.erase(it);
            return ;
        }
    }
    std::cerr << "Event not found, failed to remove" << std::endl;
}

void Timeout::reset_time(epoll_event *event)
{
   std::list<epoll_event *>::iterator it = _events.begin();
   CustomData *event_data = (CustomData *) event->data.ptr;
    for (; it != _events.end(); it++)
    {
        CustomData *li = (CustomData *) (*it)->data.ptr;
        if (li->id == event_data->id)
        {
            li->start_time = time(NULL);
            return ;
        }
    }
}

void Timeout::event_timed_out(epoll_event *event)
{
    CustomData *event_data = (CustomData *) event->data.ptr;

    close(event_data->fd);
    if (event_data->type == ft::CGI_R)
    {
        kill(event_data->pid, SIGKILL);
        Response response;
        response.process_error(HTTP_INTERNAL_SERVER_ERROR);
        send(
            event_data->cgi_fd,
            response.get_response(),
            response.get_response_size(),
            MSG_DONTWAIT
        );
        close(event_data->cgi_fd);
    }
    remove(event);
    epoll_ctl(event_data->epfd, EPOLL_CTL_DEL, event_data->fd, event);
    event_data->type = ft::TIMEOUT;
    std::cout << "Event timed out" << std::endl;
}

void Timeout::verify(void)
{
    std::list<epoll_event *>::iterator it = _events.begin();
    for (; it != _events.end(); it++)
    {
        CustomData *event_data = (CustomData *)(*it)->data.ptr;
        if (event_data->start_time + event_data->duration < time(NULL))
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