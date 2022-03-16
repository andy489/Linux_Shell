// github.com/andy489

/*
    cut -d: -f7 /etc/passwd | sort | uniq -c | sort
                            a      b         c
    
    Pseudo code for the algorithm:
    
    init1
    fork
        child "cut [..]"
    init2
    fork
        child "sort"
    init3
    fork
        child "uniq [..]"
    init4
    sort
*/

#include <unistd.h>
#include <err.h>

int main(void){
	// init1
	int a[2];
	if(pipe(a) == -1)
		err(1, "could not pipe a");

	const pid_t child_cat = fork();
	if(child_cat == -1)
		err(2, "could not fork for cut");

	if(child_cat == 0){ // child process for cut
		close(a[0]);
		if(dup2(a[1], 1) == -1)
			err(3, "could not dup2");
		if(execlp("cut", "Pesho is cutting", "-d:", "-f7", "/etc/passwd", (char *)NULL) == -1)
			err(4, "could not execlp cut with args");
	}
	close(a[1]);
	//init2
	int b[2];
	if(pipe(b) == -1)
		err(5, "could not pipe");

	const pid_t child_sort = fork();
	if(child_sort == -1)
		err(6, "could not fork for sort");

	if(child_sort == 0){ // child process for sort
		if(dup2(a[0],0) == -1)
			err(7, "could not dup2");
		close(b[0]);
		if(dup2(b[1], 1) == -1)
			err(1, "could not dup2");
		if(execlp("sort", "Pesho is sorting", (char *)NULL) == -1){
			err(8, "could not execlp sort");
		}
	}
	close(b[1]);
	//init3
	int c[2];
	if(pipe(c) == -1)
		err(9, "could not pipe c");
		
	const pid_t child_uniq = fork();
	if(child_uniq == -1)
		err(10,"could not fork for uniq");

	if(child_uniq == 0){ // child process for uniq
		if(dup2(b[0], 0) == -1)
			err(11, "could not dup2");
		close(c[0]);
		if(dup2(c[1], 1) == -1)
			err(12, "could not dup2");
		if(execlp("uniq", "Pesho filters out repeated lines and counts them", "-c", (char *)NULL) == -1){
			err(13, "could not execlp uniq with args");
		}
	}
	close(c[1]);	
	//init 4 and executing final command in parent process
	if(dup2(c[0],0) == -1)
		err(14, "could not dup");
	if(execlp("sort", "Pesho is sorting again", (char *)NULL) == -1)
		err(15, "could not exec");
}
