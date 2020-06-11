// github.com/andy489
// first solution
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

int log_line(const char *path, time_t start, time_t end, int exit_code){
	char buf[3 * sizeof(intmax_t) + 4];
	sprintf(buf, "%jd %jd %d\n", (intmax_t)start, (intmax_t)end, exit_code);
	ssize_t fd = open(path, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if(fd == -1){
		return -1;
	}
	int len = strlen(buf);
	if(write(fd, buf, len) != len){
		errno = EIO;
		return -1;
	}
	if(close(fd) == -1) return -1;
	return 0;
}

int main(int argc, char **argv){
	if(argc < 3){
		errx(1, "INvalid number of arguments. Usage: %s <sec> <Q> [args]", argv[0]);
	}

	char *duration = argv[1];
	if( *duration < '0' || *duration > '9' || duration[1]){
		errx(2, "Invalid first argument");
	} 
	
	int cond_old, cond_new = 0;
	do{
		time_t start = time(NULL);
		const pid_t p = fork();
		if(p == 0){
			if(execvp(argv[2], argv + 2) == -1){
				err(2, "could not exec");
			}
		}
	
		if(p == -1){
			err(3, "could not fork");
		}

		int status;
		if(wait(&status) == -1){
			err(4, "could not wait");
		}

		time_t end = time(NULL);
		const int exitcode = WIFEXITED(status) ? WEXITSTATUS(status) : 129;

		if(log_line("run.log", start, end, exitcode) < 0){
			err(5, "error");
		}

		cond_old = cond_new;
		cond_new = WEXITSTATUS(status) && end - start < *duration - '0';

	} while(!cond_old || !cond_new);

	exit(0);
}
