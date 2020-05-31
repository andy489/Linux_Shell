// github.com/andy489

/*
		cat file.txt | sort
			     a

	we can transform the pipeline into:

	sort file.txt

	then we will only need to execlp("sort", "sorting file", argv[1], (char *)NULL);
	but the task is to do it with pipeline
		
	Pseudo code for the algorithm:
	
	init1
	fork
		child "cat"
	init2
	sort
*/

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
	if(argc != 2){
		err(1, "Invalid number of arguments. Usage: %s <file.txt>", argv[0]);
	}

	const char *file = argv[1];
	
	const ssize_t fd_check = open(file, O_RDONLY);
	if(fd_check == -1){
		err(2, "colud not open %s in read mode", file);
	}
	
	close(fd_check);

	int a[2];
	if(pipe(a) == -1){
		err(3, "could not pipe");
	}

	const pid_t child_cat = fork();
	if(child_cat == -1){
		err(4, "could not fork");
	}
	
	if(child_cat == 0){
		// we are in child for cat process
		close(a[0]);
		if(dup2(a[1], 1) == -1){
			err(5, "could not dup");
		}
		if(execl("/bin/cat", "cat", file, (char *)NULL) == -1){
			err(6, "could not exec");
		}	
	}
	
	close(a[1]);
	
	if(dup2(a[0], 0) == -1){
		const int old_errno = errno;
		close(a[0]);
		errno = old_errno;
		err(7, "could not dup");
	}
	if(execlp("sort", "pesho_sort", (char *)NULL) == -1){
		const int old_errno = errno;
		close(a[0]);
		errno = old_errno;
		err(8, "could not exec");
	}
	// we will never get here, but just for clarity:
	exit(0);
}
