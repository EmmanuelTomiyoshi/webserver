#include "Memory.hpp"
#include "Request2.hpp"
#include "CustomData.hpp"
#include "Timeout.hpp"

std::list<CustomData *> Memory::_m_events;
std::list<char *> Memory::_m_buffers;

void Memory::add(epoll_event *addr)
{
    if (addr == NULL)
        return ;

    CustomData *data = (CustomData *) addr->data.ptr;
    if (std::find(_m_events.begin(), _m_events.end(), data) != _m_events.end())
        return ;
    _m_events.push_back(data);
}

void Memory::add(CustomData *data)
{
    if (data == NULL)
        return ;

    if (std::find(_m_events.begin(), _m_events.end(), data) != _m_events.end())
        return ;
    _m_events.push_back(data);
}

void Memory::add(char * addr)
{   
    if (addr == NULL)
        return ;

    if (std::find(_m_buffers.begin(), _m_buffers.end(), addr) != _m_buffers.end())
        return ;
    _m_buffers.push_back(addr);
}

void Memory::del(epoll_event *addr)
{
    if (addr == NULL)
        return ;

    CustomData *data = (CustomData *) addr->data.ptr;

    if (std::find(_m_events.begin(), _m_events.end(), data) == _m_events.end())
    {
        std::cout << "\n+----> addr to delete not found\n" << std::endl;
        return ;
    }
    _m_events.remove(data);
    Timeout::remove(addr);
    delete data;
    addr->data.ptr = NULL;
}

void Memory::del(CustomData *data)
{
    if (data == NULL)
        return ;

    if (std::find(_m_events.begin(), _m_events.end(), data) == _m_events.end())
    {
        std::cout << "\n+----> addr to delete not found\n" << std::endl;
        return ;
    }
    _m_events.remove(data);
    Timeout::remove(data);
    delete data;
}

void Memory::del(char *addr)
{
    _m_buffers.remove(addr);
    delete [] addr;
}

void Memory::remove(epoll_event *addr)
{
    _m_events.remove((CustomData *) addr->data.ptr);
}

void Memory::remove(CustomData *data)
{
    _m_events.remove(data);
}

void Memory::remove(char *addr)
{
    _m_buffers.remove(addr);
}

void Memory::clear_events(void)
{
    std::list<CustomData *>::iterator it;
    it = _m_events.begin();
    for (; it !=  _m_events.end(); it++)
    {
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
