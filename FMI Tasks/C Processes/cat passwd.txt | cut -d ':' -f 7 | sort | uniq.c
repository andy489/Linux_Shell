// github.com/andy489

/*
	cat ./passwd.txt | cut -d ':' -f 7 | sort | uniq
			 a		   b      c	
	since cut can accept file as argument, we can transform the pipeline into:

	cut -d: -f7 ./passwd.txt | sort | uniq
				 a      b

	now we have 2 pipes instead of 3

	init1
	fork
		child "cut"
	init2
	fork
		child "sort"
	init3
	uniq
	
*/

#include <err.h>
#include <unistd.h>

int main(void){
	int a[2];
	if(pipe(a) == -1)
		err(1, "could not pipe a");

	const pid_t child_cut = fork();
	if(child_cut == -1)
		err(2, "could not fork for cut");

	if(child_cut == 0){ // we are in child for cut
		close(a[0]);
		if(dup2(a[1],1) == -1)
			err(3, "could not dup2");
			
		if(execlp("cut", "Pesho is cutting", "-d:", "-f7", "/etc/passwd", (char *)NULL) == -1)
			err(4, "could not execlp cut with args");
	}
	close(a[1]);

	int b[2];
	if(pipe(b) == -1)
		err(5, "could not pipe b");
	
	const pid_t child_sort = fork();
	if(child_sort == -1)
		err(6, "could not fork");
	
	if(child_sort == 0){ // we are in child for sort
		if(dup2(a[0], 0) == -1)
			err(7, "could not dup2");
			
		close(b[0]);
		if(dup2(b[1],1) == -1)
			err(8, "could not dup2");

		if(execlp("sort", "Pesho is sorting", (char *)NULL) == -1)
			err(9, "could not execlp sort");
	}
	close(b[1]);
	// we are in parent process for the final command
	if(dup2(b[0], 0) == -1)
		err(10, "could not dup2");
	
	if(execlp("uniq", "Pesho filters out the repeated lines", (char *)NULL) == -1)
		err(11, "could not execlp uniq");
}
