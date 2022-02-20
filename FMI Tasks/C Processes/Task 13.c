/*
    13. Да се напише програма на C, която получава като командни параметри две команди (без параметри). 
    Изпълнява ги едновременно и извежда на стандартния изход номера на процеса на първата завършила успешно. 
    Ако нито една не завърши успешно извежда -1.
*/

#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char **argv){
	if(argc != 3)
		errx(1, "Invalid number of arguments. Usage %s <cmd1> <cmd2>", argv[0]);

	const char *cmd1 = argv[1], *cmd2 = argv[2];

	const pid_t child_pid1 = fork();
	if(child_pid1 == -1)
		err(2, "could not fork (1st child)");

	if(child_pid1 == 0) // we are in first child process
		if(execlp(cmd1, cmd1, (char *)NULL) == -1)
			err(3, "error while execlp %s", cmd1);

	pid_t child_pid2 = fork();
	if(child_pid2 == -1)
		err(4, "could not fork (2nd child)");

	if(child_pid2 == 0) // we are in second child process
		if(execlp(cmd2, cmd2, (char *)NULL) == -1)
			err(5, "error while execlp %s", cmd2);
	
	int status, flag = 0;
	const pid_t first_finished_pid = wait(&status);
	if(first_finished_pid == -1)
		err(6, "could not wait any child");
	
	if(WIFEXITED(status)){
		if(WEXITSTATUS(status) == 0){
			printf("PID of first finished process: %d\n", first_finished_pid);
			flag = 1;
		}
	}

	if(!flag){
		const pid_t try_second_pid = wait(&status);

		if(WIFEXITED(status)){
			if(WEXITSTATUS(status) == 0){
				printf("PID of first finished process: %d\n", try_second_pid);
				flag = 1;
			}
		}
	}
	
	// guarantee that the children will terminate before the parent (which is not necessary)
	if(wait(NULL) == -1)
		err(7, "could not wait children");

	if(!flag)
		printf("-1\n");

	exit(0);
}
