#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <csignal>


int daemonize(char* root, char* log_file, char * pid_file) 
{
	pid_t pid = fork();
	if (pid < 0) 
	{
		syslog(LOG_ERR | LOG_USER, "First fork failed: %m");
		return 1;
	}

	if (pid > 0) 
	{
		exit(0);
	}

	// set session leader
	setsid();

	// setup web root
	chdir(root);

	// clear permision mask
	umask(0);

	pid = fork();
	if (pid < 0) 
	{
		syslog(LOG_ERR | LOG_USER, "Second fork failed: %m");
		return 1;
	}

	if (pid > 0) 
	{
		exit(0);
	}

	// Close standart streams
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// Open new std streams to file. Assign to fd = 0
	if (open("/dev/null", O_RDONLY) < 0)  
	{
		syslog(LOG_ERR | LOG_USER, "Unable to open /dev/null: %m");
		return 1;
	}

	// Assign to fd = 1
	if (open(log_file, O_WRONLY | O_CREAT | O_APPEND, 0644) < 0) 
	{
		syslog(LOG_ERR | LOG_USER, "Unable to open %s: %m", log_file);
		return 1;
	}

	// Assign to fd = 2
    if (dup(1) < 0)
    {
      syslog(LOG_ERR | LOG_USER, "Unable to dup output descriptor: %m");
      return 1;
    }

	FILE *fp = fopen(pid_file, "w");
	pid = getpid();
	fprintf(fp, "%d\n", pid);
	fclose(fp);
}


void stop_daemon(char* pid_file) 
{
	FILE *fp = fopen(pid_file, "r");
	if (!fp) {
		printf("Cannot stop. Pid file %s not found", pid_file);
	}
	int pid = 0;
	fscanf(fp, "%d", &pid);
	fclose(fp);
	
	if (-1 == kill(pid, SIGTERM)) {
		printf("Cannot stop. Pid %d ", pid);
	}
}