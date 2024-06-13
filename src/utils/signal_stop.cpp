#include "base.hpp"
#include "Server.hpp"

static Server **store_server(void)
{
    static Server *server = NULL;
    return &server;
}

void set_server_store(Server *server)
{
    Server **store = store_server();
    *store = server;
}

void stop_server(void)
{
    Server *server = *(store_server());
    if (server == NULL)
        return ;
    
    server->stop();
}

void sig_handler(int num)
{
    (void) num;
    stop_server();
}

void set_signal_stop(void)
{
    signal(SIGINT, sig_handler);
}
