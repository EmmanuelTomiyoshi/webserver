#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include  "base.hpp"
#include "../parsing/Request.hpp"
#include "Configs.hpp"
#include "Config.hpp"

/* 
    create setters

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

        Route *_route;

        //set_path will verify if its public or not and create the right path
        std::string _path;
        std::ifstream _file;
        void open_file(void);
        void open_public_file(void);
        void open_route_file(void);

        std::string _ext;
        std::string _mime;
        std::string _type;
        void set_public_file_info(void);

        //the body can hold string or binary
        class body
        {
            public:
                char *data;
                size_t size;
                void set(char *data, size_t size);
        };

        std::string get_content(void); //get all the content from the page and close the file
        std::string create_response(void); //create a response: status code, http version and body

        void GET(void);
        void POST(void);
        void DELETE(void);

        Config *_config;

        static std::string http_version;

        bool is_public(void);


    public:
        Response(void);

        void init(char *buff, Config *config);

        std::string something(void);

        std::string process(void);

        static std::string build_response(
            std::string status_code,
            std::string body,
            std::string mime,
            size_t len
        );

        char *error(std::string status_code);

        static std::map<std::string, std::string> mime_types;
};

#endif