/*
    T1 - Да се напише програма на C, която приема аргумент - име на файл. Програмата да:
    	- записва във файла 'fo'
    	- създава child процес, който записва 'bar\n'
    	- parent-а, след като изчака child процеса, записва 'o\n'
*/

#include <stdlib.h>
#include <err.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv){
	if(argc != 2)
		errx(1, "Invalid number of arguments. Uage: %s <filepath>", argv[0]);

	const char *file = argv[1];

	ssize_t fd = open(file, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if(fd == -1)
		err(2, "could not open file %s", file);

	if(write(fd, "fo", 2) != 2){
		const int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(3, "could not write to file %s", file);
	}
	close(fd);
	
	const pid_t child = fork();
	if(child == -1)
		err(4, "could not fork process for child");
	
	if(child == 0){ // we are in child process
		fd = open(file, O_APPEND | O_WRONLY);
		if(fd == -1)
			err(5, "could not open file %s", file);

		if(write(fd, "bar\n", 4) != 4){
			const int olderrno = errno;
			close(fd);
			errno = olderrno;
			err(6, "could not write to file %s", file);
		}
		close(fd);
		exit(0);
	}

	
	if(wait(NULL) == -1)
		err(7, "could not wait for child process");

	fd = open(file, O_APPEND | O_WRONLY);
	if(fd == -1)
		err(8, "could not open file %s", file);
	
	if(write(fd, "o\n", 2) != 2){
		const int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(9, "could not write to file %s", file);
	}

	close(fd);
	exit(0);
}
