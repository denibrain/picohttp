#include <request.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>     // open
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h> // stat
#include <filetype.h> // mime

char* concat(char* buf, int size, char* suff, int suff_len) {
	if (buf) {
		buf = (char*)realloc(buf, size + suff_len + 1);
	} else {
		buf = (char*)malloc(suff_len + 1);
		size = 0;
	}
	memcpy(buf + size, suff, suff_len);
	buf[size + suff_len] = 0;
	return buf;
}

void HttpRequest::free_headers() 
{
	for(int i = 0; i < headers_count; i++) {
		free(headers[i]);
	}

	free(headers);

	headers = NULL;
}

HttpRequest::~HttpRequest() 
{
	printf("Close connection\n");

	free_headers();

	shutdown(socket_, SHUT_RDWR);
	close(socket_);	
}

int HttpRequest::parse_headers() 
{
	if (headers) {
		free_headers();
	}

	size_t buffer_tail_len = 0;
	char *buffer_tail;	

	headers_allocated = 20;
	headers = (char**)calloc(headers_allocated, sizeof(char*));
	size_t last_header_length = 0;
	size_t total_size = 0;
	size_t r;
	headers_count = 0;
	char buffer[1024];

	while((r = recv(socket_, buffer, 1024, MSG_NOSIGNAL)) > 0) {
		total_size += r;
		if (total_size > 10240) 
		{
			return 414;
		}
		char *pos;
		char *lastpos = buffer;
		size_t seach_size = r;
		while (seach_size > 0 && (pos = (char*)memchr((void*)lastpos, 10, r))) {
			size_t len = pos - lastpos;
			if (!len || (len == 1 && *(pos - 1) == 13)) {
				if (!headers[headers_count]) {
					buffer_tail_len = seach_size - len - 1;
					buffer_tail = buffer_tail_len ? pos + 1 : NULL;
					return 0;
				}
			} else {
				size_t new_header_len = *(pos - 1) == 13 ? len - 1 : len;
				headers[headers_count] = concat(headers[headers_count], last_header_length, lastpos, new_header_len);
			}

			lastpos = pos + 1; // next char after LF
			seach_size -= (len + 1);
			
			headers_count++;
			last_header_length = 0;

			if (headers_count == headers_allocated) {
				if (headers_allocated >= 100) {
					return 400;
				}
				headers_allocated += 20;
				headers = (char**) realloc(headers, headers_allocated * sizeof (char*));
			}
		}

		if (seach_size) {
			headers[headers_count] = concat(headers[headers_count], last_header_length, lastpos, seach_size);
			last_header_length += seach_size;
		}
	}

	headers_count++;
	return 0;
}

void HttpRequest::process() 
{
	char buffer[1024];
	size_t r;
	char method[16];
	size_t content_length = 0;

	// read headers
	int last = 0;
	int error_code = 0;

	if ((error_code = parse_headers()) || !headers_count)
	{
		send_error(error_code ? error_code : 400);
		return;
	}

	printf("Headers: %d\n", headers_count);
	for(int i=0; i<headers_count;i++) printf("%d: %s\n", i, headers[i]);

	char * request = headers[0];
	char * pos = strchr(request, 32);
	int len = pos - request;
	if (!pos || len > 15) 
	{
		send_error(405);
		return;
	}

	memcpy(method, request, len);
	method[len] = 0;

	printf("Got method %s\n", method);
	// read body or ignore
	if (!strcmp(method, "GET")) {
		method_ = GET;
	} else 
	if (!strcmp(method, "POST")) {
		method_ = POST;


	} else {
		send_error(405);
		return;
	}

	char* uri = pos + 1; 
	pos = strchr(uri, 32);
	len = pos - uri;
	
	if (!pos)  // HTTP/0.9
	{ 
		http_version = 9;
	} 
	else
	{
		http_version = 10;

		*pos = 0; // cut version
		pos++;
		if (strcmp(pos, "HTTP/1.0")) {
			send_error(505);
			return;
		}
	}

	printf("Got uri %s\n", uri);

	len = strlen(uri);

	if (len > 10240) {
		send_error(414);
	}

	int root_len = strlen(root_);
	char* path = (char*)malloc(root_len + len + 1);

	memcpy(path, root_, root_len);
	memcpy(path + root_len, uri, len);
	path[root_len + len] = 0;

    send_file(path);
	free(path);
}

int HttpRequest::file_exists(char *path, size_t &size) 
{
    struct stat f_stat; 
    if ( stat(path, &f_stat) )
    {
        return 0;
    }
    if ( !S_ISREG(f_stat.st_mode) )
    {
        return 0;
    }
    size = f_stat.st_size;
    return 1;	
}

const char* HttpRequest::send_status(int status) 
{
	int statuses[] = {200, 400, 404, 405, 411, 413, 
		414, 500, 505};
	const char* status_messages[9] = {"OK", "Bad Request", "Not Found", "Method Not Allowed", "Length Required", "Request Entity Too Large", 
	"Request-URI Too Long", "Internal Server Error", "HTTP Version Not Supported"};
	int status_count = 9;

	const char *status_text = NULL;
	for(int i = 0; i < status_count; i++) {
		if (statuses[i] == status) {
			status_text = status_messages[i];
			break;
		}
	}
	printf("Send status %s\n", status_text);

	if (http_version == 10) {
		char buffer[64];
		sprintf(buffer, "HTTP/1.0 %d %s\r\n", status, status_text);
		send_text(buffer);
	}

	return status_text;
}

void HttpRequest::send_error(int error) {
	printf("Send error %d\n", error);
	const char* status_text = send_status(error);

	if (http_version == 10) {
		HttpResponse response;
		response.length = strlen(status_text);
		response.type = "text/html; charset=iso-8859-1";

		send_headers(response);
	}

	send_text(status_text);
}

void HttpRequest::send_text(const char *text) {
	send(socket_, text, strlen(text), MSG_NOSIGNAL);
}

void HttpRequest::send_file(char *file_name) 
{
  	size_t file_size;
  	if (!file_exists(file_name, file_size)) {
  		send_error(404);
  		return;
    }

	if (http_version == 10) {
		send_status(200);
		HttpResponse response;
		response.length = file_size;
		response.type = get_content_type(file_name);
		send_headers(response);
	}

	int fd = open(file_name, O_RDONLY);
	send_file_data(fd);
	close(fd);
}

void HttpRequest::send_file_data(int fd) {
	size_t rd;
	char buffer[1024];
	while(rd = read(fd, buffer, 1024)) {
		send(socket_, buffer, rd, MSG_NOSIGNAL);
	}
}

const char* HttpRequest::get_content_type(char* path) {
	return FileType::get_instance()->get_file_type(path);
}

void HttpRequest::send_headers(HttpResponse response)  {
	char hd[128];
	char endl[] = "\r\n";
	if (response.length) {
		sprintf(hd, "Content-Length: %ld\r\n", response.length);
		send(socket_, hd, strlen(hd), MSG_NOSIGNAL);
	}

	if (response.type) {
		sprintf(hd, "Content-Type: %s\r\n", response.type);
		send(socket_, hd, strlen(hd), MSG_NOSIGNAL);
	}
	send(socket_, endl, 2, MSG_NOSIGNAL);
}
