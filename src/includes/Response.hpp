#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include  "base.hpp"
#include "../parsing/Request.hpp"
#include "Configs.hpp"
#include "Config.hpp"
#include "error_codes.hpp"
#include "Request2.hpp"
#include "CGI.hpp"

/* 
    create setters -> OK

    open file 

    read_file:
        if public read from public //read_public
        else read from root + location //read_route
        return file

    //build_response() using the properties
    //send_response() public method must be called


    implement char *ft::read_file(std::string path)
    implement error function
 */

class Response
{
    private:
        Request _req;
        Request2 _request;
        char *_buff;
        size_t _buff_size;

        Route *_route;

        //set_path will verify if its public or not and create the right path
        std::string _status;
        std::string _path;
        std::ifstream _file;
        void open_file(void);
        void open_public_file(void);
        void open_route_file(void);

        std::string _ext;
        std::string _mime;
        std::string _type;
        void set_public_file_info(void);
        void show_file_info(void);

        void read_binary(void);
        void read_text(void);
        void fill_body(void);

        char *_http_response;
        size_t _http_response_size;
        void create_response(void);

        void build_error(std::string code);

        //the body can hold string or binary
        class Body
        {
            public:
                Body(void);
                ~Body(void);
                char *data;
                size_t size;
        };

        Body _body;

        void GET(void);
        void POST(void);
        void DELETE(void);

        Config *_config;

        static std::string http_version;

        bool is_public(void);

        void execute(void);
        void execute_error(std::string code);

    public:
        Response(char *buff, size_t size, Config *config);

        ssize_t send_response(int fd);

        static std::map<std::string, std::string> mime_types;
};

#endif