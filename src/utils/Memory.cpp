#include "Memory.hpp"
#include "Request2.hpp"
#include "CustomData.hpp"

std::list<epoll_event *> Memory::_m_events;
std::list<char *> Memory::_m_buffers;

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
    if (addr == NULL)
        return ;
    _m_events.remove(addr);
    CustomData *data = (CustomData *) addr->data.ptr;
    delete data;
    delete addr;
}

void Memory::del(char *addr)
{
    _m_buffers.remove(addr);
    delete  addr;
}

void Memory::remove(epoll_event *addr)
{
    _m_events.remove(addr);
}

void Memory::remove(char *addr)
{
    _m_buffers.remove(addr);

}

void Memory::clear_events(void)
{
    std::list<epoll_event *>::iterator it;
    it = _m_events.begin();
    for (; it !=  _m_events.end(); it++)
    {
        CustomData *data = (CustomData *) (*it)->data.ptr;
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
