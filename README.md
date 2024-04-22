# webserver

## TO-DO
 - criar classe Server que encapsulará toda a complexidade de configurar socket e iniciar listen
 - estudar as funções do pdf
 - definir logica pra escutar em várias portas

---

## NOTES
What does the parser need to have

- Header fields and values
- Content-Length
- Request method
- Response status code
- Transfer-Encoding
- HTTP version
- Request URL
- Message body


### Other notes

1) HTTP Methods:
	Correct: GET, POST, PUT, DELETE, HEAD, OPTIONS, TRACE
	Incorrect: get, PoSt, head, delete (case-sensitive, must be uppercase)
2) URI Structure:
	Correct: /path/to/resource, /api/user?id=123
	Incorrect: /../../etc/passwd, ?id=123 (missing leading / in absolute path)
	# If str[0] != '/' or uri.empty().
	# Invalid characters are: spaces, control characters, $, |, < and >
	# reject urls that contain path traversal sequences such as '..' or '/../'
	# Validate URI Length, limitate to a maximum URI length (such as 2048 chars)
3) HTTP Versioning:
	Correct: HTTP/1.1, HTTP/1.0
	Incorrect: http/1.1 (case-sensitive, must be uppercase)
	# Must also check which versions our webserver accepts

4) Message Headers:
	Correct (mandatory): Host: www.example.com, Content-Type: application/json, Content-Length: 123
	Incorrect: (missing mandatory headers): Missing Content-Type header
	# validate mandatory headers, host and content length must be present.
6) Status Codes:
	Correct: 200 OK, 404 Not Found, 500 Internal Server Error
	Incorrect: 200, not found, Server Error (incorrect format or missing status phrase)
7) Prevent Header Injection:
	Ensure that headers like User-Agent, Referer, and Host do not contain newline characters or other special characters that could be used for injection attacks.
8) Prevent Buffer Overflow Attacks:
	Validate the Content-Length header against the actual length of the message body.
9) URI Syntax:
	Understand the URI syntax components: scheme, authority, path, query, and fragment.
	Example: https://www.example.com/path?query=value#fragment
10) URI Normalization
	Convert Uppercase Letters to Lowercase:
		Input URI: HTTP://www.EXAMPLE.com/Path/To/Resource
		Normalized URI: http://www.example.com/Path/To/Resource
	Remove Dot-Segments (Path Normalization):
		Input URI: http://www.example.com/../a/b/../c/./d.html
		Normalized URI: http://www.example.com/a/c/d.html
	Remove Default Ports:
		Input URI: https://www.example.com:443/path
		Normalized URI: https://www.example.com/path
11) 