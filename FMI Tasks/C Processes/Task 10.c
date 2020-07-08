/*
    10. Да се напише програма на C, която получава като параметри от команден ред две команди (без параметри). 
    Изпълнява първата. Ако тя е завършила успешно изпълнява втората. Ако не, завършва с код -1.
*/

// github.com/andy489
#include <stdlib.h>
#include <err.h>
#include <unistd.h>

int main(int argc, char **argv){
	if(argc != 3)
		errx(1, "Invalid number of arguments. Usage: %s <cmd1> <cmd2>", argv[0]);

	const char *cmd1 = argv[1], *cmd2 = argv[2];
	
	const pid_t cmd1_pid = fork();
	if(cmd1_pid == -1)
		err(2, "could not fork for %s", cmd1);
	
	if(cmd1_pid == 0) // we are in child for cmd1
		if(execlp(cmd1, cmd1, (char*)NULL) == -1)
			err(3, "could not execlp %s", cmd1);

	int cmd1_stat;
	if(wait(&cmd1_stat) == -1)
		err(4, "could not wait child process for command %s", cmd1);

	if(!WIFEXITED(cmd1_stat))
		errx(-1, "%s did not terminated normally", cmd1);

	if(WEXITSTATUS(cmd1_stat))
		errx(-1, "%s exit status not 0", cmd1); 
		
	const pid_t cmd2_pid = fork();
	if(cmd2_pid == -1)
		err(5, "could not fork for %s", cmd2);

	if(cmd2_pid == 0) // we are in child for cmd2
		if(execlp(cmd2, cmd2, (char*)NULL) == -1)
			err(6, "could not execlp %s", cmd2);

	wait(NULL);
	exit(0);
}
