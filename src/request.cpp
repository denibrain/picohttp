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
	printf("Concat!");
	if (buf) {
		printf("C-1");	
		buf = (char*)realloc(buf, size + suff_len + 1);
	} else {
		printf("C-0");	
		buf = (char*)malloc(suff_len + 1);
		size = 0;
	}
	printf("Memcpy %d %d\n", size, suff_len);
	memcpy(buf + size, suff, suff_len);
	buf[size + suff_len] = 0;
	printf("MemcpyF\n");
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

	printf("Ready to read headers\n");

	while((r = recv(socket_, buffer, 1024, MSG_NOSIGNAL)) > 0) {
		total_size += r;
		if (total_size > 10240) 
		{
			return 414;
		}
		printf("Read: %ld [%s]\n", total_size, buffer);

		char *pos;
		char *lastpos = buffer;
		size_t seach_size = r;
		while (seach_size > 0 && (pos = (char*)memchr((void*)lastpos, 10, r))) {
			size_t len = pos - lastpos;
			printf("Found, len: %ld\n", len);
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

	printf("Search uri\n");

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
	//char exts[][] = {"png", "jpg", "gif", "svg", "html", "pdf", }
	return FileType::get_instance().get_file_type(path);
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
// setlogmask (LOG_UPTO (LOG_NOTICE));
// openlog ("exampleprog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
// syslog (LOG_NOTICE, "Program started by User %d", getuid ());
// syslog (LOG_INFO, "A tree falls in a forest");
// closelog ();

// GET /p/writing-an-echo-server-in-libev-and-c/375?23490239409230423904 HTTP/1.1
// Host: www.skitoy.com
// Connection: keep-alive
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.109 Safari/537.36
// Accept-Encoding: gzip, deflate, sdch
// Accept-Language: en-US,en;q=0.8,fr;q=0.6,ru;q=0.4
// Cookie: _ga=GA1.2.1838284241.1455992530

// GET /home/about/11.html HTTP/1.1
// Host: www.cnzexpress.com
// Connection: keep-alive
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.109 Safari/537.36
// Referer: http://www.cnzexpress.com/track.php
// Accept-Encoding: gzip, deflate, sdch
// Accept-Language: en-US,en;q=0.8,fr;q=0.6,ru;q=0.4
// Cookie: a2192_pages=4; a2192_times=7

// HTTP/1.1 200 OK
// Date: Sun, 21 Feb 2016 17:25:44 GMT
// Last-Modified: Wed, 13 Sep 2000 14:31:48 GMT
// ETag: "238e6-3712b40649d00"
// Accept-Ranges: bytes
// Content-Length: 145638
// Cache-Control: max-age=21600
// Expires: Sun, 21 Feb 2016 23:25:44 GMT
// P3P: policyref="http://www.w3.org/2014/08/p3p.xml"
// Content-Type: text/html; charset=iso-8859-1
// Strict-Transport-Security: max-age=604800; includeSubdomains; preload
// Content-Security-Policy: upgrade-insecure-requests

// HTTP/1.1 304 Not Modified
// Date: Sun, 21 Feb 2016 17:35:47 GMT
// ETag: "238e6-3712b40649d00"
// Expires: Sun, 21 Feb 2016 23:35:47 GMT
// Cache-Control: max-age=21600
// Strict-Transport-Security: max-age=604800; includeSubdomains; preload
// Content-Security-Policy: upgrade-insecure-requests

