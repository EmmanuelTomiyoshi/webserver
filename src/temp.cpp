#include "CGI.hpp"
#include "base.hpp"
#include "./parsing/Request.hpp"

void set_post(CGI & cgi)
{
	cgi.set_content_type("multipart/form-data; boundary=---------------------------1900715293566303716132296103");
	cgi.set_request_method("POST");
	cgi.set_script_name("./cgi-bin/perl_test.pl");
	char body[610000];
	int pid = open("../body", O_RDONLY);
	size_t bytes = read(pid, body, 610000);
	cgi.set_body(body);
	cgi.set_content_length(ft::int_to_str(bytes));
}

void set_get(CGI & cgi)
{
	cgi.set_content_type("text/html");
	cgi.set_request_method("GET");
	cgi.set_script_name("./cgi-bin/hello.pl");
}



void test(void)
{
    char data[20000];
    int pid = open("../headers", O_RDONLY);
	read(pid, data, 20000);
	Request req;
	req.init(data);
	req.info();
	std::string body = req.get_body();
	std::cout << "body: " << body << std::endl;
}

void temp(void)
{
    test();
	// CGI cgi;
    // set_get(cgi);
	// cgi.info();
	// cgi.execute();
	exit(0);
}
