/*
    T3 - Напишете програма на C, която демонстрира употребата на dup/dup2 между parent и child процеси. 
    Parent-ът трябва да изпраща стринга, получен като първи аргумент на командния ред към child-а, 
    където той да може да се чете от stdin. Child процесът да изпълнява wc -c.
*/

#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(const int argc, const char * const argv[]){
	if(argc !=2 )
		errx(1, "Invalid number of arguments. Usage: %s <string>", argv[0]);
	const char *str = argv[1];
	int p[2];

	if(pipe(p) == -1)
		err(2, "could not create pipe");

	const pid_t child_pid = fork();
	if(child_pid == -1)
		err(3, "could not fork");

	if(child_pid == 0){ // we are in child process
		close(p[1]);
		close(0);
		dup(p[0]);
		// dup2(pf[0], 0);
		if(execlp("wc", "wc", "-c", (char *)NULL) == -1)
			err(4, "could not exec");
	}

	close(p[0]);
	ssize_t len = (ssize_t)strlen(str);
	if(write(p[1], str, len) != len)
		err(5, "error while writing in parent process into pipe");

	close(p[1]);
	wait(NULL);

	exit(0);
}
