#include "Timeout.hpp"
# include "Response.hpp"
#include "CustomData.hpp"
#include "Memory.hpp"
#include "Server.hpp"

std::list<CustomData *> Timeout::_events;

Server *Timeout::server = NULL;

void Timeout::add(epoll_event *event)
{
    CustomData *event_data = (CustomData *) event->data.ptr;

    _events.push_back(event_data);
}

void Timeout::remove(epoll_event *event)
{
   CustomData *event_data = (CustomData *) event->data.ptr;

    _events.remove(event_data);
}

void Timeout::remove(CustomData *data)
{
    _events.remove(data);
}

void Timeout::reset_time(epoll_event *event)
{
   CustomData *event_data = (CustomData *) event->data.ptr;

    event_data->start_time = time(NULL);
}

void Timeout::event_timed_out(CustomData *event_data)
{
    close(event_data->fd);
    if (event_data->type == ft::CGI_R)
    {
        kill(event_data->pid, SIGKILL);
        if (Timeout::server)
        {
            Timeout::server->create_error_event(event_data->cgi_fd, HTTP_INTERNAL_SERVER_ERROR);
        }
    }
    _events.remove(event_data);
    epoll_event event;
    event.data.ptr = event_data;
    epoll_ctl(event_data->epfd, EPOLL_CTL_DEL, event_data->fd, &event);
    event_data->type = ft::TIMEOUT;
    Memory::del(&event);
    std::cout << "Event timed out" << std::endl;
}

void Timeout::verify(void)
{
    std::list<CustomData *>::iterator it = _events.begin();
    for (; it != _events.end(); it++)
    {
        if ((*it)->start_time + (*it)->duration < time(NULL))
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
