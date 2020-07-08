/*
    T1 - Да се напише програма на C, която приема аргумент - име на файл. Програмата да:
    	- записва във файла 'fo'
    	- създава child процес, който записва 'bar\n'
    	- parent-а, след като изчака child процеса, записва 'o\n'
*/

// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
	if(argc!=2)
		errx(1, "Invalid number of arguments. Usage: %s <filepath>", argv[0]);

	const char *file = argv[1];

	ssize_t fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if(fd == -1)
		err(2, "error while opening file %s", file);

	const char *str1 = "foo\n", *str2 = "bar\n";

	if(write(fd, str1, 2) != 2){
		const int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(3, "error while writing into file %s", file);
	}
	close(fd);

	const pid_t child_pid = fork();
	if(child_pid == -1)
		err(4, "could not fork");

	if(child_pid == 0){
		fd = open(filename, O_WRONLY | O_APPEND);
		if(write(fd, str2, 4) != 4){
			const int olderrno = errno;
			close(fd);
			errno = olderrno;
			err(5, "failed to write into file %s in child process", file);
		}
		exit(0);
	}

	int child_status;
	const pid_t wait_code = wait(&child_status);
	
	if(wait_code == -1){
		const int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(6, "could not wait for child");
	}

	if(!WIFEXITED(child_status)){
		const int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(7, "child did not terminate normally");
	}

	if(WEXITSTATUS(child_status) != 0){
		err(8, "child exit code not 0, file should be already closed");
	}
	
	fd = open(filename, O_WRONLY | O_APPEND);
	if(write(fd, str1 + 2, 2) != 2){
		const int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(9, "failed to write into file %s in parent process", file);
	}
	
	close(fd);
	exit(0);
}
