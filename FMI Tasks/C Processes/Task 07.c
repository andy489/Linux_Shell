/*
    7. Да се напише програма на С, която получава като параметри три команди (без параметри), 
    изпълнява ги последователно, като изчаква края на всяка и извежда на стандартния изход 
    номера на завършилия процес, както и неговия код на завършване.
*/

// github.com/andy489
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv){
	if(argc!=4){
		errx(1,"Invalid number of arguments. Usage %s <cmd1> <cmd2> <cmd3>", argv[0]);
	}

	const char *cmd1 = argv[1], *cmd2 = argv[2], *cmd3 = argv[3];

	const pid_t child1 = fork();
	if(child1 == -1){
		err(2,"could not fork first process");
	}

	if(child1 == 0){
	// we are in first child
		if(execlp(cmd1, "pesho_1", (char *)NULL) == -1){
			err(3,"failed to execlp %s", cmd1);
		}
	}

	int status1;
	const pid_t wait_code1 = wait(&status1);
	if(wait_code1 == -1) {
		err(4,"could not wait first child");
	}

	if(WIFEXITED(status1)){
		printf("first child, pid: %d, exit code: %d\n", child1, WEXITSTATUS(status1));
	}

	const pid_t child2 = fork();
	if(child2 == -1){
		err(5,"could not fork second process");
	}
	
	if(child2 == 0){
	// we are in second child
		if(execlp(cmd2, "pesho_2", (char *)NULL) == -1){
			err(6,"failed to execlp %s", cmd2);
		}
	}

	int status2;
	const pid_t wait_code2 = wait(&status2);
	if(wait_code2 == -1){
		err(7,"could not wait secont process");
	}

	if(WIFEXITED(status2)){	
		printf("second child, pid: %d, exit code: %d\n", child2, WEXITSTATUS(status2));
	}

	const pid_t child3 = fork();
	if(child3 == -1){
		err(8,"could not fork third process");
	}

	if(child3 == 0){
	// we are in third child
		if(execlp(cmd3, "pesho_3", (char *)NULL) == -1){
			err(9,"failed to execlp %s", cmd3);
		}
	}

	int status3;
	const pid_t wait_code3 = wait(&status3);
	if(wait_code3 == -1){
		err(10,"could not wait thord child");
	}

	if(WIFEXITED(status1)){
		printf("third child, pid: %d, exit code: %d\n", child3, WEXITSTATUS(status3));
	}

	printf("finished\n");
	exit(0);
}
