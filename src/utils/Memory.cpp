#include "Memory.hpp"
#include "Request2.hpp"

std::list<epoll_event *> Memory::_m_events;
std::list<char *> Memory::_m_buffers;

Memory::Memory(void)
{

}

void Memory::add(epoll_event *addr)
{
    if (addr == NULL)
        return ;

    _m_events.push_back(addr);
}

void Memory::add(char * addr)
{   
    if (addr == NULL)
        return ;

    _m_buffers.push_back(addr);
}

void Memory::del(epoll_event *addr)
{
    (void) addr;
}

void Memory::del(char *addr)
{
    _m_buffers.remove(addr);
    delete  addr;
}

void Memory::remove(epoll_event *addr)
{
 (void) addr;
}

void Memory::remove(char *addr)
{
    _m_buffers.remove(addr);

}

void Memory::clear_events(void)
{
    ft::CustomData *data;
    std::list<epoll_event *>::iterator it;
    it = _m_events.begin();
    for (; it !=  _m_events.end(); it++)
    {
        data = (ft::CustomData *) (*it)->data.ptr;
        if (data->request != NULL)
            delete data->request;
        delete data;
        delete (*it);
    }
    _m_events.clear();
}

void Memory::clear_buffers(void)
{
    std::list<char *>::iterator it;
    it = _m_buffers.begin();
    for (; it !=  _m_buffers.end(); it++)
    {
        delete [] (*it);
    }
    _m_buffers.clear();
}

void Memory::clear(void)
{
    clear_buffers();
    clear_events();
}
