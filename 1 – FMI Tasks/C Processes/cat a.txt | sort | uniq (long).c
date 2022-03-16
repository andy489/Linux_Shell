/*
	cat a.txt | sort | uniq
		  a      b
			
	init1
	fork
		child "cat"
	init2
	fork
		child "sort"
	init3
	uniq
*/

#include <stdlib.h>
#include <unistd.h>
#include <err.h>

int main(void){
	int a[2];
	if(pipe(a) == -1)
		err(1, "could not pipe a");

	pid_t child_pid = fork();
	if(child_pid == -1)
		err(2, "could not fork");

	if(child_pid == 0){ // we are in child "cat"
		close(a[0]);
		dup2(a[1], 1); // TODO: check dup2
		if(execlp("cat", "cat", "a.txt", (char *)NULL) == -1)
			err(3, "could not exec");
	}
	close(a[1]);
	
	int b[2];
	if(pipe(b) == -1)
		err(4, "could not pipe b");
	
	child_pid = fork();
	if(child_pid == -1)
		err(5, "could not fork");

	if(child_pid == 0){ // we are in child "sort"
		dup2(a[0], 0); // TODO: check dup2
		close(b[0]);
		dup2(b[1],1); // TODO: check dup2
		if(execlp("sort", "sort", (char *)NULL) == -1)
			err(6, "could not exec");
	}
	
	close(b[1]);
	dup2(b[0],0); // TODO: check dup2

	if(execlp("uniq", "uniq", (char *)NULL) == -1)
		err(7, "could not exec");
}
