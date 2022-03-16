// github.com/andy489

/*
	cat a.txt | sort | uniq
		  a      b
	
	since sort works with files, we can rearrange the pipeline like this:

	sort a.txt | uniq
		   a
		
	init1
	fork
		child "sort"
	init2
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

	if(child_pid == 0){ // we are in child "sort"
		close(a[0]);
		close(1); // to be more precise
		dup(a[1]); // TODO: check dup
		
		if(execlp("sort", "sort", "a.txt", (char *)NULL) == -1)
			err(3, "could not exec");
	}
	close(a[1]);
	close(0); // to be more precise
	dup(a[0]); // TODO: check dup

	if(execlp("uniq", "uniq", (char *)NULL) == -1)
		err(4, "could not exec");
}
