/*
    6. Да се напише програма на С, която получава като параметър команда (без параметри) 
    и при успешното ѝ изпълнение, извежда на стандартния изход името на командата.
*/

// github.com/andy489
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char **argv){
	if(argc!=2){
		errx(1,"Invalid number of arguments. Usage: %s <cmd>", argv[0]);
	}

	const char *cmd = argv[1];
	
	const pid_t child_pid = fork();
	if(child_pid == -1){
		err(2, "failed to fork");
	}
	
	if(child_pid == 0){ // we are in child process
		if(execlp(cmd, "pesho_no_drama", (char *)NULL) == -1){
			err(3,"failed to execlp %s", cmd);
		}
	}
	
	int child_status;
	const pid_t wait_code = wait(&child_status);

	if(wait_code == -1){
		err(4,"could not wait for child");
	}

	if(!WIFEXITED(child_status)){
		err(5,"child did not terminate normally");
	}

	if(WEXITSTATUS(child_status)){
		err(6,"child exit code not 0");
	}
	
	printf("command %s was successfully executed\n", cmd);

	exit(0);
}
