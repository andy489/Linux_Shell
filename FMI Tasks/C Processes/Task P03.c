/*
    T3 - Напишете програма на C, която демонстрира употребата на dup/dup2 между parent и child процеси. 
    Parent-ът трябва да изпраща стринга, получен като първи аргумент на командния ред към child-а, 
    където той да може да се чете от stdin. Child процесът да изпълнява wc -c.
*/

// github.com/andy489
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(const int argc, const char * const argv[]){
	if(argc !=2 ){
		errx(1, "Invalid number of arguments. Usage: %s <string>", argv[0]);
	}

	int pf[2];

	if(pipe(pf) == -1){
		err(2, "could not create pipe");
	}

	const pid_t child_pid = fork();
	if(child_pid == -1){
		err(3, "could not fork");
	}

	if(child_pid == 0){
		// we are in child process
		close(pf[1]);
		close(0);
		dup(pf[0]);
		// dup2(pf[0], 0);
		if(execlp("wc", "wc", "-c", (char *)NULL) == -1){
			err(4, "could not exec");
		}
	}

	close(pf[0]);
	ssize_t len = strlen(argv[1]);
	ssize_t wr_sz = write(pf[1], argv[1], len);
	if(wr_sz != len){
		err(5, "error while writing in parent process into pipe");
	}
	close(pf[1]);
	wait(NULL);

	exit(0);
}
