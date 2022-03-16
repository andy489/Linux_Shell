// 2nd solution

#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

int execChild(char *args[]){
	int pid = fork();
	if(pid == -1){
		err(1, "could not fork");
	}
	if(pid == 0){
		if(execvp(args[0], args) == -1){
			err(2, "could not exec");
		}
	}
	int status;
	wait(&status);
	if(WIFEXITED(status))
		return WEXITSTATUS(status);
	return 1;
}

void flog(time_t start, time_t end, int status){
	int fd = open("log.txt", O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP);
	if(fd == -1){
		err(4, "could not open file log.txt");
	}
	char logline[30];
	
	ssize_t len = 0;

	sprintf(logline, "%ld %ld %d\n", start, end, status);
	len = strlen(logline);
	if(write(fd, logline, len) != len){
		err(5, "could not write in log.txt");
	}
	
	close(fd);
}

bool execute(int lim, char *args[]){
	time_t start = time(NULL);
	int status = execChild(args); 	
	time_t end = time(NULL);

	flog(start, end, status);

	return (((end - start) < lim) && status !=0);
}


int main(int argc, char **argv){
	if(argc < 3){
		errx(3, "Invalid number of arguments. Usage: %s <number> <cmd> [args]", argv[0]);
	}	

	char c = argv[1][0];

	if(c > '9' || c < '0' || argv[1][1] != '\0')
		errx(99, "Invalid digit argument"); 

	int sec = (argv[1][0] - '0');

	

	char *args[12];

	int i = 2, cnt = 0;
	for(; i < argc; ++i){
		args[cnt++] = argv[i];
	}
	args[cnt] = NULL;

	int condcnt = 0;	
	while(condcnt < 2) {
		if(execute(sec, args) != 0)
			condcnt++;
		else
			condcnt = 0;
	}
	exit(0);
}
