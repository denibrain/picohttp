#include <server.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <daemon.h>
#include <filetype.h>

HttpServer* server;

static void stop_signal_handler(int signal_no) {
	server->stop();
}

int main(int argc, char ** argv) 
{
	char pid_file[] = "/tmp/final.pid";
	char log_file[] = "/tmp/final.log";
	unsigned short port = 12345;
	char ip[64] = "0.0.0.0";
	char root[1024];
	int opt;

	if (!getcwd(root, 1024)) {
		printf("Cannot get work directory\n");
		exit(1);
	}

	while ((opt = getopt(argc, argv, "sh:p:d:")) != -1) {
		switch(opt) {
			case 's':
				stop_daemon(pid_file);
				exit(0);
				break;

			case 'h':
				if (strlen(optarg) > 63) 
				{
					printf("Invalid IP parameter\n");
					exit(1);
				}
				strcpy(ip, optarg);
				break;

			case 'p': 
				if (strlen(optarg) > 10) 
				{
					printf("Invalid port parameter\n");
					exit(1);
				}
				port = atoi(optarg);
				break;

			case 'd':
				if (strlen(optarg) > 1023) 
				{
					printf("Invalid web root parameter\n");
					exit(1);
				}
				strcpy(root, optarg);
				break;

			case '?':
               fprintf(stderr, "Usage: %s -h host -p port -d root\n", argv[0]);
               exit(1);
		}
	}

	daemonize(root, log_file, pid_file);

	FileType::get_instance()->init();

  	server = new HttpServer(ip, port, root);
    signal(SIGTERM, stop_signal_handler);
    signal(SIGINT, stop_signal_handler);

    server->run(); 
    delete server;
}