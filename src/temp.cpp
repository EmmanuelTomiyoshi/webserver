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

void test2(void)
{
	char something[] = "123456789";

	char *test = something + 5;

	std::cout << something << std::endl;
	std::cout << test << std::endl;
	int info_size = (test - something);
	std::cout << "info_size: " << info_size << std::endl;
	std::cout << "body_size: " << (std::strlen(something) - info_size) << std::endl;
}

void test_php(void)
{
	const char *argv[] = { "/bin/php", "./cgi-bin/test.php", NULL };
	execve("/bin/php", (char *const *) argv, NULL);
	perror(NULL);
	std::cout << "error executing\n";
}

/* 

GATEWAY_INTERFACE=CGI/1.1
SERVER_PROTOCOL=HTTP/1.1
REDIRECT_STATUS=200
REQUEST_METHOD=POST
CONTENT_LENGTH=247
CONTENT_TYPE=multipart/form-data; boundary=---------------------------120183001110947073403483897981
SCRIPT_FILENAME=./cgi-bin/upload_debug.pl
 */

void test_perl(void)
{
	int fd = open("./debug", O_RDONLY);
	char buff[20000];
	size_t bytes = read(fd, buff, 20000);
	close(fd);

	std::cout << "content----:" << std::endl;
	write(1, buff, bytes);
	std::cout << "end content----:\n" << std::endl;

	const char *argv[] = { "/bin/perl", "./cgi-bin/upload_debug.pl", NULL };
    const char *envp[] = {
        "GATEWAY_INTERFACE=CGI/1.1",
        "SERVER_PROTOCOL=HTTP/1.1",
        "REDIRECT_STATUS=200",
        "REQUEST_METHOD=POST",
        "CONTENT_LENGTH=247",
        "CONTENT_TYPE=multipart/form-data; boundary=---------------------------120183001110947073403483897981",
        "SCRIPT_FILENAME=./cgi-bin/upload_debug.pl",
        NULL // Terminate the array with a null pointer
    };

	int pfds[2];
	pipe(pfds);

	int pid = fork();
	if (pid == 0)
	{
		write(pfds[1], buff, bytes);
		dup2(pfds[0], 0);
		close(pfds[0]);
		close(pfds[1]);
		execve("/bin/perl", (char *const *) argv, (char *const *) envp);
		perror(NULL);
		std::cout << "error executing\n";
		exit(0);
	}
	wait(NULL);
	close(pfds[0]);
	close(pfds[1]);
}

#include "./utils/Timeout.hpp"
void timeout_test(void)
{
	Timeout timeout(1000);
}

void temp(void)
{
	timeout_test();
	exit(0);
}
