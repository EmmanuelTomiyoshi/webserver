#ifndef RESPONSE_HPP
# define RESPONSE_HPP
#include  "base.hpp"
#include "../parsing/Request.hpp"
#include "Configs.hpp"
#include "Config.hpp"

class Response
{
    private:
        Request _req;

        std::ifstream *_file;
        void open_page(void); //try to open pages until some is opened or the try list ends
        std::string get_content(void); //get all the content from the page and close the file
        std::string create_response(void); //create a response: status code, http version and body

        std::string GET(void);
        void POST(void);
        void DELETE(void);

        Config *_config;

        static std::string http_version;

    public:
        Response(void);

        void init(char *buff, Config *config);

        std::string something(void);

        std::string process(void);

        static std::string build_response(
            std::string status_code,
            std::string body
        );
};

#endif