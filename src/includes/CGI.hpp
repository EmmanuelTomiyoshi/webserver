#ifndef CGI_HPP
# define CGI_HPP
# include "base.hpp"
# include "ft.hpp"
# include "error_codes.hpp"

// # define CGI_PROGRAM "/bin/python3"
# define gateway_interface "GATEWAY_INTERFACE=CGI/1.1"
# define server_protocol "SERVER_PROTOCOL=HTTP/1.1"
# define query_s "QUERY_STRING="
# define ENVS_SIZE 20
# define ARGV_SIZE 3
# define CGI_BIN "/bin/perl"
# define W 1
# define R 0

class CGI
{
    public:
        class ResponseData {
            public:
                char *body;
                size_t body_size;
                std::string content_type;

                char *http_response;
                size_t http_response_size;
        };

    private:
        std::string _request_method;
        std::string _request_method_raw;
        std::string _content_type;
        std::string _content_length;
        std::string _content_length_raw;
        std::string _query_string;
        std::string _script_name;
        std::string _script_name_raw;
        static std::string _gateway_interface;
        static std::string _server_protocol;

        const char *_envs[ENVS_SIZE];
        const char *_argv[ARGV_SIZE];

        char *_body;
        size_t _body_size;

        char *_response;
        size_t _response_size;

        bool error(void);
        void add_env(const char *value);
        void create_envp(void);
        void create_argv(void);

        void dup_pfds(void);

        void write_body(void);
        void read_response(void);

        int _pfds_a[2];
        int _pfds_b[2];
        int _pid;

        void execute_cgi_script(void);

        void extract_response_data(void);
        void extract_content_type(size_t header_size);

        void format_http_response(void);

        ResponseData _response_data;

    public:
        CGI(void);

        void set_request_method(std::string value);
        void set_content_type(std::string value);
        void set_content_length(std::string value);
        void set_query_string(std::string value);
        void set_script_name(std::string value);
        void set_body(char *value);
        void set_body_size(size_t value);

        void execute(void);
        void info(void);
        char *get_response(void);
        size_t get_response_size(void);


};

#endif