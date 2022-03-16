// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
	if(argc!=2){
		errx(1, "Invalid number of arguments. Usage %s <string>", argv[0]);
	}

	const char *str = argv[1];

	int pf[2];
	if(pipe(pf) == -1){
		errx(2, "could not create pipe");
	}

	const pid_t child_pid = fork();
	if(child_pid == -1){
		err(3, "could not fork child process");
	}

	if(child_pid == 0){
		// we are in child process
		close(pf[1]);
		char buf;
		ssize_t read_size = read(pf[0], &buf, 1);
		while(read_size == 1){
			write(1, &buf, 1);
			read_size = read(pf[0], &buf, 1);
		}
		if(read_size == -1){
			err(4,"error while reading in child process");
		}
		close(pf[0]);
		exit(0);
	}
	
	close(pf[0]);
	ssize_t len = (ssize_t)strlen(str);
	if(write(pf[1], str, len) != len){
		err(4, "error while writing in parent");
	}
	//close(pf[1]); // uncomment here to remove deadlock
	wait(NULL);
	exit(0);
}
