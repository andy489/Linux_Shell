/*
    13. Да се напише програма на C, която получава като командни параметри две команди (без параметри). 
    Изпълнява ги едновременно и извежда на стандартния изход номера на процеса на първата завършила успешно. 
    Ако нито една не завърши успешно извежда -1.
*/

// github.com/andy489
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv){
	if(argc != 3)
		errx(1, "Invalid number of arguments. Usage: %s <cmd1> <cmd2>", argv[0]);

	const char *cmd1 = argv[1], *cmd2 = argv[2];


	const pid_t child1 = fork();
	if(child1 == -1)
		err(2, "could not fork for child 1");

	if(child1 == 0) // we are in child 1 
		if(execlp(cmd1, cmd1, (char*)NULL) == -1)
			err(3, "could not execlp %s", cmd1);

	const pid_t child2 = fork();
	if(child2 == -1)
		err(4, "could not fork for child 2");
	
	if(child2 == 0)
		if(execlp(cmd2, cmd2, (char*)NULL) == -1)
			err(5, "could not execlp %s", cmd2);


	int status1, status2;
	
	printf("%s: %d\n", cmd1, child1);
	printf("%s: %d\n", cmd2, child2);

	const pid_t first = wait(&status1);
	if(first == -1)
		err(6, "could not wait for any process");

	if(WIFEXITED(status1) && !WEXITSTATUS(status1)){
		printf("%d\n", first);
	} else { // the first one has finished but not successsfully
		const pid_t second = wait(&status2);
		if(second == -1)
			err(7, "could not wait for second process");
	
		if(WIFEXITED(status2) && !WEXITSTATUS(status2)){
			printf("%d\n", second);
		} else printf("%d\n", -1);
	}
	exit(0);
}
