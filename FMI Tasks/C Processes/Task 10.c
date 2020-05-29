/*
    10. Да се напише програма на C, която получава като параметри от команден ред две команди (без параметри). 
    Изпълнява първата. Ако тя е завършила успешно изпълнява втората. Ако не, завършва с код -1.
*/

// github.com/andy489
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv){
	if(argc != 3){
		errx(1, "Invalid number of arguments. Usage: %s <cmd1> <cmd2>", argv[0]);
	}

	const char *cmd1 = argv[1], *cmd2 = argv[2];

	const pid_t child_pid1 = fork();
	if(child_pid1 == -1){
		err(2, "could not fork (1st child)");
	}

	if(child_pid1 == 0){
		// we are in first child
		if(execlp(cmd1, "pesho_no_drama", (char *)NULL) == -1){
			err(3, "error while execlp (1st child)");
		}
	}

	int status;
	const pid_t wait_code1 = wait(&status);
	if(wait_code1 == -1){
		err(4, "could not wait for 1st child");
	}

	if(!WIFEXITED(status)){
		warnx("first command do no terminate normally");
		exit(-1);
	}

	if(WEXITSTATUS(status) != 0){
		warnx("exit code of first command was not 0");
		exit(-1);
	}

	const pid_t child_pid2 = fork();
	if(child_pid2 == -1){
		err(7, "could not fork (2nd child)");
	}

	if(child_pid2 == 0){
		// we are in second child
		if(execlp(cmd2, "pesho_without_drama", (char *)NULL) == -1){
			err(8, "error while execlp (2nd child)");
		}
	}

	const pid_t	wait_code2 = wait(&status);
	if(wait_code2 == -1){
		err(9, "could not wait for 2nd child");
	}	

	exit(0);
}
