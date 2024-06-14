#include "BuffList.hpp"

BuffList::BuffData::BuffData(void) : buff(NULL), size(0), count(0)
{
}

BuffList::BuffList(void)
{
}

BuffList::~BuffList(void)
{
    this->clear();
}

void BuffList::add(char *buff, ssize_t size)
{
    BuffList::BuffData node;
    node.buff = buff;
    node.size = size;
    node.count = 0;
    _buff_list.push_back(node);
}

BuffList::BuffData & BuffList::front(void)
{
    return this->_buff_list.front();
}

BuffList::BuffData & BuffList::back(void)
{
    return this->_buff_list.back();
}

void BuffList::del_front(void)
{
    BuffList::BuffData & node = _buff_list.front();
    if (node.buff)
        delete [] node.buff;
    _buff_list.pop_front();
}

void BuffList::del_back(void)
{
    BuffList::BuffData & node = _buff_list.back();
    if (node.buff)
        delete [] node.buff;
    _buff_list.pop_back();
}

void BuffList::clear(void)
{
    while (_buff_list.empty() == false)
    {
        del_front();
    }
}
