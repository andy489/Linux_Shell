// github.com/andy489

/*
	find <diripath> -type f -printf "%T@:%p"| sort -nr | head -1 | cut -d: -f2
	                                        a    	     b	     c
	Pseudo code for the algorithm:

	init1
	fork
		child "find [..]"
	init2
	fork
		child "sort [..]"
	init3
	fork
		child "head [..]"
	init4
	cut [..]
*/

#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char **argv){
	if(argc != 2)
		errx(1, "Invalid number of arguments. Usage %s <dirpath>", argv[0]);

	const char *dir = argv[1];
	
	struct stat st;
	if(stat(dir, &st) == -1)
		err(2, "could not stat %s", dir);
	
	if(S_ISDIR(st.st_mode) == 0)
		errx(3, "%s is not a valid directory name", dir);
	// init1
	int a[2];
	if(pipe(a) == -1)
		err(4, "could not pipe a");

	const pid_t child_find = fork();
	if(child_find == -1)
		err(5, "could not fork for find");

	if(child_find == 0){ // we are in child for "fing"
		close(a[0]);
		if(dup2(a[1], 1) == -1)
			err(6, "could not dup2");

		if(execlp("gfind", "Pesho is searching", dir, "-type", "f", "-printf", "%T@:%p\n", (char *)NULL) == -1)
			err(7, "could not execlp find with args");
	}
	close(a[1]);
	// init2
	int b[2];
	if(pipe(b) == -1)
		err(8, "could not pipe b");

	const pid_t child_sort = fork();
	if(child_sort == -1)
		err(9, "could not fork for sort");

	if(child_sort == 0){ // we are in child for "sort"
		if(dup2(a[0], 0) == -1)
			err(10, "could not dup2");

		close(b[0]);
		if(dup2(b[1], 1) == -1)
			err(11, "could not dup2");
	
		if(execlp("sort", "Pesho is sorting", "-nr", (char *)NULL) == -1){
			err(12, "could not execlp sort with args");
		}
	}	
	close(b[1]);
	close(a[0]);
	// init3
	int c[2];
	if(pipe(c) == -1)
		err(13, "could not pipe c");
	
	const pid_t child_head = fork();
	if(child_head == -1)
		err(14, "could not fork c");

	if(child_head == 0){ // we are in child for "head"
		if(dup2(b[0], 0) == -1)
			err(16, "could not dup2");

		close(c[0]);
		if(dup2(c[1], 1) == -1)
			err(17, "could not dup2");
	
		if(execlp("head", "Pesho takes the top result", "-1", (char *)NULL) == -1)
			err(18, "could not execlp head with args");
	}
	close(c[1]);
	close(b[0]);
	//init4
	if(dup2(c[0], 0) == -1)
		err(19, "could not dup2");

	if(execlp("cut", "Pesho is cutting", "-d:", "-f2", (char *)NULL) == -1){
		err(20, "could not execlp cut with args");
	}
}
