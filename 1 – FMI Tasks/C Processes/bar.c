#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv){
	if(argc !=2)
		errx(1, "Invalid number of arguments. Usage %s <cmd>", argv[0]);
	
	const char *cmd = argv[1];

	int fdp = open("mypipe", O_RDONLY);
	if(fdp == -1)
		err(2, "could not open named pipe");

	const pid_t child = fork();
	if(child == -1)
		err(3, "could not fork");

	if(child == 0){ // child process
		if(dup2(fdp, 0) == -1)
			err(3, "could not dup2");

		if(execlp(cmd, cmd, (char *)NULL) == -1)
			err(4, "could not execlp");
	}
	
	if(child > 0){ // parent process
		if(wait(NULL) == -1)
			err(5, "could not wait for child");
		
		if(unlink("mypipe") == -1)
			err(6, "could not remove named pipe");
		exit(0);
	}
}
