#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include  "base.hpp"
#include "Configs.hpp"
#include "Config.hpp"
#include "error_codes.hpp"
#include "Request2.hpp"
#include "CGI.hpp"
#include "Timeout.hpp"

class Response
{
    private:
        Request2 *_request;
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
        void open_route_file_default(void);
        void open_route_file_upload(void);

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
        void replace_error_path(std::string code);
        void build_default_error(void);

        std::string _location;
        void set_redirect(void);

        //the body can hold string or binary
        class Body
        {
            public:
                Body(void);
                ~Body(void);
                char *data;
                ssize_t size;
                bool should_free;
        };

        Body _body;

        void GET(void);
        void GET_normal(void);
        void autoindex(void);
        void GET_cgi(void);
        void POST(void);
        void DELETE(void);

        Config *_config;
        epoll_event *_event;

        static std::string http_version;

        bool is_public(void);

        void execute(void);
        void execute_error(std::string code);

        void create_cors_response(void);
        
        void start_mimes(void);
        void fill_mime(std::string file);

        static void create_writing_event(epoll_event *old_event, char *buff, ssize_t size);
    public:
        Response(void);
        Response(epoll_event *event);
        ~Response(void);

        ssize_t send_response(void);

        static std::map<std::string, std::string> mime_types;

        void process_error(std::string code);
        char *get_response(void);
        ssize_t get_response_size(void);
};

#endif