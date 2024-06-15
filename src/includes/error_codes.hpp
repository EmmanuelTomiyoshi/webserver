#ifndef ERROR_CODES_HPP
# define ERROR_CODES_HPP

#define HTTP_OK "200"
#define HTTP_BAD_REQUEST "400"
#define HTTP_NOT_FOUND "404"
#define HTTP_METHOD_NOT_ALLOWED "405"
#define HTTP_PAYLOAD_TOO_LARGE "413"
#define HTTP_INTERNAL_SERVER_ERROR "500"
#define HTTP_SERVICE_UNAVAILABLE "503"

#include <string>

# define DEFAULT_ERROR_MESSAGE "<!DOCTYPE html>\n" \
    "<html lang=\"en\">\n" \
    "<head>\n" \
    "    <meta charset=\"UTF-8\">\n" \
    "    <title>Error</title>\n" \
    "    <style>\n" \
    "        body {\n" \
    "            font-family: Arial, sans-serif;\n" \
    "            background-color: #f8d7da;\n" \
    "            color: #721c24;\n" \
    "            text-align: center;\n" \
    "            padding: 50px;\n" \
    "        }\n" \
    "        .container {\n" \
    "            background-color: #f5c6cb;\n" \
    "            padding: 20px;\n" \
    "            border-radius: 5px;\n" \
    "            display: inline-block;\n" \
    "        }\n" \
    "        h1 {\n" \
    "            font-size: 24px;\n" \
    "        }\n" \
    "        p {\n" \
    "            font-size: 18px;\n" \
    "        }\n" \
    "    </style>\n" \
    "</head>\n" \
    "<body>\n" \
    "    <div class=\"container\">\n" \
    "        <h1>Error</h1>\n" \
    "        <p>Something went wrong. Please try again later.</p>\n" \
    "    </div>\n" \
    "</body>\n" \
    "</html>";


#endif