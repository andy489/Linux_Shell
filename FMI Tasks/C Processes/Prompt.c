// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define max_args 8
#define max_symbols 16

int main(void){
	OUTER:
	while(1){
		const char *prompt = "Andy's prompt: ";
		ssize_t len = strlen(prompt);
		if(write(1, prompt, len) != len)
			err(1, "could not write prompt msg to stdout");

		char cmd_and_args[max_args + 1][max_symbols + 1];
	
		char buf[1<<8];
		int indx = 0, row = 0, n = 0; // n is number of args + cmd (here we count them)
		ssize_t read_sz;
		while((read_sz = read(0, buf + indx, 1)) == 1 && row <= max_args && *(buf + indx) != '\n'){
			if(*(buf + indx) == ' ' || *(buf + indx) == '\n'){
				++n;
				if(n >= 7){
					warnx("too much arguments");
					goto OUTER;
				}
				buf[indx] = '\0';
				strcpy(cmd_and_args[row++], buf);
				indx = 0;
			} else {
				++indx;
				if(indx > max_symbols){
					warnx("command or argument is too long");
					goto OUTER;
				}
			}
		}
		if(read_sz == -1)
			err(3, "could not read from stdin");
		
		if(*(buf + indx) == '\n'){
			++n; // don't forget to count here 
			buf[indx] = '\0';
			if(strcmp(buf, "exit") == 0) 
				exit(0);
			strcpy(cmd_and_args[row++], buf);
		} 
				
		cmd_and_args[row][0]= '\0';	

		// here we adjust the arguments for the execvp command	
		char *cmd = cmd_and_args[0];
		char *args[n + 1];

		for(int i = 0; i < n; ++i)
			args[i] = cmd_and_args[i];
		
		args[n] = NULL;
		// arguments arе now in the required format
		
		const pid_t child = fork();
		if(child == -1)
			err(4, "could not fork");

		if(child == 0) // we are in child process for execution of the cmd with args
			if(execvp(cmd, args) == -1)
				err(5, "could not execvp");

		if(wait(NULL) == -1)
			err(6, "could not wait for child process");
	}
	exit(0);
}
