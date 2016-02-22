#ifndef __SERVER_H_INCLUDED__ 
#define __SERVER_H_INCLUDED__ 

#include <ev++.h>

class HttpServer {
public:
    HttpServer(char* ip, unsigned short port, char *dir);
	~HttpServer();
    void run();
    void stop();
	static void* request_process_caller(void *param);
	void accept_connection(ev::io &watcher, int revents);
	void request_handle(ev::io &watcher, int revents);

private:
	char terminated_;
	char* root_;
	char* ip_;
	int port_;
	ev::default_loop loop;
	ev::io accept_;


	int init_connection();
	void handle_events(int socket);
};

#endif 