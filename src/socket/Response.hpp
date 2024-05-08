#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include  "base.hpp"
#include "../parsing/Request.hpp"

class Response
{
    private:
        Request _req;
    public:
        Response (void *ptr);
        Response(std::string request);
        Response(void);

        void init(char *buff);

        std::string something(void);

        std::string get(void);
};

#endif