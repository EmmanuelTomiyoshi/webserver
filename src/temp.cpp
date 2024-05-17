#include "CGI.hpp"
#include "base.hpp"

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

void temp(void)
{
	CGI cgi;
    set_get(cgi);
	cgi.info();
	cgi.execute();
	exit(0);
}
