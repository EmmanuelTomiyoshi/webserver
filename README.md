# webserver

## THINGS TO-DO:
	- TODO: delete method
	- TODO: verify if any exception or error could crash the server
	- TODO: handle signals
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
	<!---
	# If str[0] != '/' or uri.empty().
	# Invalid characters are: spaces, control characters, $, |, < and >
	# reject urls that contain path traversal sequences such as '..' or '/../'
	# Validate URI Length, limitate to a maximum URI length (such as 2048 chars)
	-->
3) HTTP Versioning:
	Correct: HTTP/1.1, HTTP/1.0
	Incorrect: http/1.1 (case-sensitive, must be uppercase)
	<!---
	# Must also check which versions our webserver accepts
	-->

4) Message Headers:
	Correct (mandatory): Host: www.example.com, Content-Type: application/json, Content-Length: 123
	Incorrect: (missing mandatory headers): Missing Content-Type header
	<!---
	# validate mandatory headers, host and content length must be present.
	-->
6) Status Codes:
	Correct: 200 OK, 404 Not Found, 500 Internal Server Error
	Incorrect: 200, not found, Server Error (incorrect format or missing status phrase)

------

7) Content length is optional. GET doesn't have content length, for example