#include "CustomData.hpp"
#include "Request2.hpp"
#include "Timeout.hpp"

CustomData::CustomData(void) : config(NULL),
request(NULL), buff(NULL), timeout(NULL),
start_time(0), duration(5), fd(-1), cgi_fd(-1),
epfd(-1), pid(-1), buff_size(0), w_count(0)
{

}

CustomData::~CustomData(void)
{
    if (this->buff)
        delete this->buff;
    if (this->request)
        delete this->request;
    // if (this->id >= 0)
    //     this->timeout->remove(this->id);
    
}

