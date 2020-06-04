// github.com/andy489

/*
	find dir -type f -printf "%T@:%p"| sort -nr | head -1 | cut -d: -f2
	                                  a    	     b	       c
	Pseudo code for the algorithm:

	init1
	fork
		child "gfind"
	init2
	fork
		child "sort"
	init3
	fork
		child "head"
	init4
	cut
*/

#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv){
	if(argc != 2){
		errx(1, "Invalid number of arguments. Usage %s <dirname>", argv[0]);
	} 
	const char *dir = argv[1];
		
	int a[2];
	if(pipe(a) == -1){
		err(2, "could not pipe a");
	}

	const pid_t child_find = fork();
	if(child_find == -1){
		err(3, "could not fork a");
	}

	if(child_find == 0){
		// we are in child "find"
		close(a[0]);
		if(dup2(a[1], 1) == -1){
			err(4, "could not dup2");
		}
		if(execlp("find", "pesho is searching", dir, "-type", "f", "-printf", "%T@:%p\n", (char *)NULL) == -1){
			err(5, "could not exec in a");
		}
	}
	close(a[1]);
	// ---- ---- ---- ----
	int b[2];
	if(pipe(b) == -1){
		err(6, "could not pipe b");
	} 
	const pid_t child_sort = fork();
	if(child_sort == -1){
		err(7, "could not fork b");
	}

	if(child_sort == 0){
		// we are in child sort
		if(dup2(a[0], 0) == -1){
			err(8, "could not dup");
		}
		close(b[0]);
		if(dup2(b[1], 1) == -1){
			err(9, "could not dup");
		}
		if(execlp("sort", "pesho is sorting", "-nr", (char *)NULL) == -1){
			err(10, "could not exec in b");
		}
	}	
	close(b[1]);
	close(a[0]);
	// ---- ---- ---- ---- 
	int c[2];
	if(pipe(c) == -1){
		err(11, "could not pipe c");
	}
	
	const pid_t child_head = fork();
	if(child_head == -1){
		err(12, "could not fork c");
	}

	if(child_head == 0){
		// we are in child_head
		if(dup2(b[0], 0) == -1){
			err(13, "could not dup");
		}
		close(c[0]);
		if(dup2(c[1], 1) == -1){
			err(14, "could not dup");
		}
		if(execlp("head", "pesho takes the top result", "-1", (char *)NULL) == -1){
			err(15, "could not exec c");
		}
	}
	close(c[1]);
	close(b[0]);
	if(dup2(c[0], 0) == -1){
		err(16, "could not dup");
	}
	if(execlp("cut", "pesho is cutting", "-d:", "-f2", (char *)NULL) == -1){
		err(17, "could not exec d");
	}

	// we should never be here, but just for clarity
	exit(0);
	}
