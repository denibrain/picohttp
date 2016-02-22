#ifndef __DAEMON_H_INCLUDED__ 
#define __DAEMON_H_INCLUDED__ 

int daemonize(char* root, char* log_file, char* pid_file);
void stop_daemon(char* pid_file);

#endif