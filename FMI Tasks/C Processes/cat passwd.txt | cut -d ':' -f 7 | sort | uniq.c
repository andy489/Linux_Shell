// github.com/andy489

/*
	cat ./passwd.txt | cut -d ':' -f 7 | sort | uniq
					
	we transform the command pipeline into

	cut -d: -f7 ./passwd.txt | sort | uniq
				 a      b

	since cut can accept files as argument

	init1
	fork
		child "cut"
	init2
	fork
		child "sort"
	init3
	uniq
	
*/

#include <stdlib.h>
#include <err.h>
#include <unistd.h>

int main(void){
	int a[2];
	if(pipe(a) == -1)
		err(1, "could not pipe");

	const pid_t child_pid1 = fork();
	if(child_pid1 == -1)
		err(2, "could not fork");

	if(child_pid1 == 0){
		close(a[0]);
		if(dup2(a[1],1) == -1)
			err(3, "could not dup");
			
		if(execlp("cut", "Pesho is cutting", "-d:", "-f7", "./passwd.txt", (char *)NULL) == -1)
			err(4, "could not exec");
	}
	close(a[1]);

	int b[2];
	if(pipe(b) == -1)
		err(5, "could not pipe");
	
	const pid_t child_pid2 = fork();
	if(child_pid2 == -1)
		err(6, "could not fork");
	
	if(child_pid2 == 0){
		if(dup2(a[0], 0) == -1)
			err(7, "could not dup");
			
		close(b[0]);
		if(dup2(b[1],1) == -1)
			err(8, "could not dup");

		if(execlp("sort", "Stamat is sorting", (char *)NULL) == -1)
			err(9, "could not sort");
	}
	close(b[1]);
	
	if(dup2(b[0], 0) == -1)
		err(10, "could not dup");
	
	if(execlp("uniq", "Gosho filters out the repeated lines", (char *)NULL) == -1)
		err(11, "could not exec");
}
