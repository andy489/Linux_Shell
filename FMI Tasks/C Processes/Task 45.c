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
#include <sys/types.h>
#include <err.h>

int main(void){
	int a[2];
	if(pipe(a) == -1){
		err(1, "could not pipe");
	}

	const pid_t child_cat = fork();
	if(child_cat == -1){
		err(2, "could not fork");
	}

	if(child_cat == 0){
		// child process cat
		close(a[0]);
		if(dup2(a[1], 1) == -1){
			err(3, "could not dup");
		}
		if(execlp("cut", "yea baby, Pesho is cutting", "-d:", "-f7", "/etc/passwd", (char *)NULL) == -1){
			err(4, "could not exec");
		}	
	}
	close(a[1]);
	// ---- ---- ----
	int b[2];
	if(pipe(b) == -1){
		err(5, "could not pipe");
	}

	const pid_t child_sort = fork();
	if(child_sort == -1){
		err(6, "could not fork");
	}

	if(child_sort == 0){
		// child process sort
		if(dup2(a[0],0) == -1){
			err(7, "could not dup");
		}
		close(b[0]);
		if(dup2(b[1], 1) == -1){
			err(1, "could not dup");
		}
		if(execlp("sort", "now Pesho is sorting", (char *)NULL) == -1){
			err(8, "could not exec");
		}
	}
	close(b[1]);
	// ---- ---- ----
	int c[2];
	if(pipe(c) == -1){
		err(9, "could not pipe");
	}
		
	const pid_t child_uniq = fork();
	if(child_uniq == -1){
		err(10,"could not fork");
	}

	if(child_uniq == 0){
		// child process uniq
		if(dup2(b[0], 0) == -1){
			err(11, "could not dup");
		}
		close(c[0]);
		if(dup2(c[1], 1) == -1){
			err(12, "could not dup");
		}
		if(execlp("uniq", "unique that sh*t and count it, Pesho!", "-c", (char *)NULL) == -1){
			err(13, "could not exec");
		}
	}
	close(c[1]);	
	// ---- ---- ----
	if(dup2(c[0],0) == -1){
		err(14, "could not dup");
	}	
	if(execlp("sort", "Pesho is about to do his final move...", (char *)NULL) == -1){
		err(15, "could not exec");
	}
}
