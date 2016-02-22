#ifndef __REQUEST_H_INCLUDED__ 
#define __REQUEST_H_INCLUDED__ 

#include <cstdio>

struct HttpResponse {
	const char* type;
	size_t length;
	int status;
};


class HttpRequest  
{
public:
	HttpRequest(const char *root, int socket): root_ (root), socket_ (socket), headers (NULL) {}
	~HttpRequest();
	void process();

	enum {
		GET = 1, POST = 2
	};

private:
	int method_;
	int socket_;
	const char* root_;

	char** headers;
	int headers_count;
	int headers_allocated;
	int http_version;

	void free_headers();
	int parse_headers();

	int file_exists(char *path, size_t &size);
	const char* send_status(int status);
	void send_error(int error);
	void send_text(const char *text);
	void send_file(char *file_name);
	void send_file_data(int fd);
	const char* get_content_type(char* path);
	void send_headers(HttpResponse response);
};

#endif