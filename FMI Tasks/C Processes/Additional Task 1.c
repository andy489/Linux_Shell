// github.com/andy489

#include <stdlib.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char **argv){
	if(argc !=3)
		errx(1, "Invalid number of arguments. Usage: %s <cmd1> <cmd2>", argv[0]);

   const char *cmd1 = argv[1], *cmd2 = argv[2];

	int a[2];
	if(pipe(a) == -1)
		err(2, "could not pipe a");

	const pid_t child_cmd1 = fork();
	if(child_cmd1 == -1)
		err(3, "could not fork");

	if(child_cmd1 == 0){ // we are in child for exec cmd1
		if(dup2(a[0], 0) == -1)
			err(4, "could not dup2");

		close(a[1]);

		if(execlp(cmd1, cmd1, (char*)NULL) == -1)
			err(5, "could not execlp %s", cmd1);
	} 
	close(a[0]);

	int b[2];
	if(pipe(b) == -1)
		err(6, "could not pipe b");

	const pid_t child_cmd2 = fork();
	if(child_cmd2 == -1)
		err(7, "could not fork");

	if(child_cmd2 == 0){ // we are in child for exec cmd2
		if(dup2(b[0], 0) == -1)
			err(8, "could not execlp %s", cmd2);
		close(b[1]);

		if(execlp(cmd2, cmd2, (char*)NULL) == -1)
			err(9, "could not execlp %s", cmd2);
	}
	close(b[0]);
	
	char buf[1<<10];
	ssize_t read_sz;
	int row = 1;
	while((read_sz = read(0, &buf, sizeof(buf))) > 0){
		if((row & 1) == 0) // odd
			if(write(b[1], &buf, read_sz)!= read_sz)
				err(10, "could not write to \"write end\" of pipe b");
		if((row & 1) == 1) // even
			if(write(a[1], &buf, read_sz)!= read_sz)
				err(11, "could not write to \"write end\" of pipe a");
		++row;
	}
	close(a[1]);
	close(b[1]);
	exit(0);
}
