// github.com/andy489

/*
	cat file.txt | sort
	             a

	we can transform the pipeline into:

	sort file.txt

	then we will only need to execlp("sort", "sorting file", argv[1], (char *)NULL);
	But the task wants us to do it with pipeline, so:
		
	Pseudo code for the algorithm:
	
	init1
	fork
		child "cat"
	init2
	sort
*/

#include <err.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
	if(argc != 2)
		err(1, "Invalid number of arguments. Usage: %s <filepath>", argv[0]);

	const char *file = argv[1];
	
	const ssize_t fd_check = open(file, O_RDONLY);
	if(fd_check == -1)
		err(2, "colud not open %s in read mode", file);
	
	close(fd_check);

	int a[2];
	if(pipe(a) == -1)
		err(3, "could not pipe a");

	const pid_t child_cat = fork();
	if(child_cat == -1)
		err(4, "could not fork");
	
	if(child_cat == 0){ // we are in child for cat
		close(a[0]);
		if(dup2(a[1], 1) == -1)
			err(5, "could not dup2");
			
		if(execl("/bin/cat", "cat", file, (char *)NULL) == -1)
			err(6, "could not execl cat");	
	}
	close(a[1]);
	
	if(dup2(a[0], 0) == -1)
		err(7, "could not dup2");
	
	if(execlp("sort", "pesho_sort", (char *)NULL) == -1)
		err(8, "could not exec");
}
