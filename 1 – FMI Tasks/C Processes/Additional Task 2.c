#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h> // for debug

void execute(char **args, int cnt){
	const pid_t child = fork();
	if(child == -1)
		err(3, "could not fork");
	if(child == 0)// child process
		if(execvp(args[0], args) == -1)
			err(4, "could not execvp %s with args", args[0]);
	if(child > 0){ // parent procrss
		if(wait(NULL) == -1)
			err(5, "could not wait for child");
		for(int i = 1; i < cnt; ++i)
			free(args[i]);
	}
}

int main(int argc, char **argv){
	if(argc != 2)
		errx(1, "Invalid number of arguments. Usage: %s <cmd>", argv[0]);		

	char *cmd = argv[1];

	char buf[1<<7];
	int read_sz;
	while((read_sz = read(0, &buf, sizeof(buf))) > 0){
		int opts = 0; // options
		for(int i = 0; i < read_sz; ++i)
			if(*(buf + i) == ' ' || *(buf + i) == '\n')
				++opts;

		char *args[opts + 1 + 1]; // +1 for cmd, +1 for last NULL
		int cnt = 0; // position of opt
		args[cnt++] = cmd;
		int indx = 0;

		char opt[1<<6];
		for(int i = 0; i < read_sz; ++i){
			if(*(buf + i) == ' ' || *(buf + i) == '\n'){
				opt[indx] = '\0';
				args[cnt] = malloc(indx + 1);
				if(args[cnt] == NULL)
					err(2, "not enough memory");
				indx = 0;
				strcpy(args[cnt++], opt);
				if(*(buf + i) == '\n'){
					args[cnt] = NULL;
					execute(args, cnt);
				}
			} else{
				opt[indx++] = *(buf + i);
			}
		}
	}
	if(read_sz == -1)
		err(99, "could not read from stdin");

	exit(0);
}
