// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define arg_size 4
#define max_args 2

void print_arr(char * arr){
	for(int i = 0; arr[i] != '\0'; ++i)
		write(1, arr + i, 1);
	
	write(1, " ", 1);
}

void print_cmd_args(char args[][arg_size+1]){
	for(int i = 0; args[i][0] != '\0'; ++i)
		print_arr(args[i]);

	write(1, "\n", 1); 
}

void print_exec_args(char *args[max_args + 1]){
	for(int i = 0; args[i] != NULL; ++i)
		print_arr(args[i]);
	write(1, "\n", 1); 
}

void exec_cmd(char args[][arg_size + 1], char *cmd){
	pid_t pid = fork();
	if(pid == 0){
		char *execv_args[max_args + 2];
		execv_args[0] = cmd;

		int i = 1, j = 0;
		while(args[j][0] != '\0')
			execv_args[i++] = args[j++];

		execv_args[i] = NULL;
		if(execvp(cmd, execv_args) == -1)
			err(11, "could not exec");
		return;		
	}
	wait(NULL);
}

int main(int argc, char **argv){
	char cmd[arg_size + 1];	
	if(argc == 1)
		strcpy(cmd, "echo");
	else if(argc == 2 && strlen(cmd) < 5)
		strcpy(cmd, argv[1]);
	else
		errx(1, "Invalid number of arguments. Usage: %s [cmd]", argv[0]);
	char arg[arg_size + 1];
	int indx = 0, row = 0;
	char cmd_args[max_args + 1][arg_size + 1];
	ssize_t read_size = -1;
	while(((read_size = read(0, arg + indx, 1)) > 0) && indx <= arg_size){
		if(row == max_args){
			cmd_args[max_args][0] = '\0';
			exec_cmd(cmd_args, cmd);
			row = 0;
		}	
		if(arg[indx] == '\n' || arg[indx] == ' '){
			arg[indx] = '\0'; 
			strcpy(cmd_args[row++], arg);
			indx = 0;
		} else {
			++indx;
		}
	}

	if(read_size == -1)
		err(2, "could not read from stdin");

	if(indx > arg_size)
		errx(3, "argument out of bounds");

	if(row != 0){
		cmd_args[row][0] = '\0';
		exec_cmd(cmd_args, cmd);
	}
	exit(0);
}
