#ifndef CGI_HPP
# define CGI_HPP
# include "base.hpp"

# define CGI_PROGRAM "/bin/python3"
# define gateway_interface "GATEWAY_INTERFACE=CGI/1.1"
# define server_protocol "SERVER_PROTOCOL=HTTP/1.1"
# define query_s "QUERY_STRING="
# define ENVS_SIZE 10
# define ARGV_SIZE 3 

class CGI
{
    private:
        std::string _request_method;
        std::string _content_type;
        std::string _content_length;
        std::string _query_string;
        std::string _script_name;
        std::string _script_name_raw;

        char **_envp;

        const char *_envs[ENVS_SIZE];

        const char *_argv[ARGV_SIZE];

        bool error(void);
        void add_env(const char *value);
        void create_envp(void);
        void create_argv(void);

        void execute_get(void);

    public:
        CGI(void);

        void set_request_method(std::string value);
        void set_content_type(std::string value);
        void set_content_length(std::string value);
        void set_query_string(std::string value);
        void set_script_name(std::string value);

        void execute(void);
        void info(void);
};

#endif