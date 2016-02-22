#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ev++.h>
#include <pthread.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include <server.h>
#include <request.h>

// todo on syslogs
#include <stdio.h>

HttpServer::HttpServer(char *ip, unsigned short port, char *root) {
	port_ = port;
	
	ip_ = (char*)malloc(strlen(ip) + 1);
	strcpy(ip_, ip);

	root_ = (char*)malloc(strlen(root) + 1);
	strcpy(root_, root);
}

HttpServer::~HttpServer() {
	free(ip_);
	free(root_);
}

void HttpServer::run() {
	 printf("Start server on %s:%d on %s\n", ip_, port_, root_);

	int server_socket = init_connection();

	handle_events(server_socket);

    shutdown(server_socket, SHUT_RDWR);
	close(server_socket);

	printf("Server is stopped\n");
}

void HttpServer::stop() {
	printf("Stoping server...\n");
	accept_.stop();
	loop.break_loop(ev::ALL);
}

// private section

int HttpServer::init_connection() {
 	int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_);
	inet_pton(AF_INET, ip_, &(addr.sin_addr));

	bind(sd, (struct sockaddr *)&addr, sizeof(addr));

	int flags;
	#if defined(O_NONBLOCK) 
		if (-1 == (flags = fcntl(sd, F_GETFL, 0)))
			flags = 0;
		fcntl(sd, F_SETFL, flags | O_NONBLOCK);
	#else
		flags = 1;
		ioctrl(sd, FIONBIO, &flags);
	#endif

	listen(sd, SOMAXCONN);

	return sd;
}

void HttpServer::accept_connection(ev::io &watcher, int revents) {
	printf("Accept new request\n");

	int client_sd = accept(watcher.fd, 0, 0);

	ev::io *client = new ev::io;

	client->set<HttpServer, &HttpServer::request_handle>(this);
	client->start(client_sd, ev::READ);
}

void HttpServer::request_handle(ev::io &watcher, int revents) {
	printf("Accept new data\n");

	watcher.stop();

	HttpRequest *request = new HttpRequest(root_, watcher.fd);

	pthread_t t;
	pthread_attr_t attr; 
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); 
    pthread_create(&t, &attr, &request_process_caller, (HttpRequest*)request);	
}

void* HttpServer::request_process_caller(void *param) {
	HttpRequest* requst = (HttpRequest*)param;
	requst->process();
	delete requst;
	pthread_exit(NULL);
}

void HttpServer::handle_events(int socket) {
	accept_.set<HttpServer, &HttpServer::accept_connection>(this);
	accept_.start(socket, ev::READ);

	loop.run(0);
}

