/*
    8. Да се напише програма на С, която получава като параметър име на файл. 
    Създава детински процес, който записва стринга foobar във файла (ако не съществува, го създава, 
    в противен случай го занулява), след което процеса родител прочита записаното във файла съдържание 
    и го извежда на стандартния изход, добавяйки по един интервал между всеки два символа.
*/

#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv){
	if(argc != 2)
		errx(1, "Invalid number of arguments. Usage: %s <filepath>", argv[0]);

	const char *file = argv[1];

	pid_t child = fork();
	if(child == -1)
		err(4, "could not fork");

	if(child == 0){ // we are in child process
		ssize_t fd = open(file, O_CREAT | O_TRUNC | O_RDWR, 0644);
		if(fd == -1)
			err(5, "could not open file %s", file);

		const char *str = "foobar";
		ssize_t len = (ssize_t)strlen(str);

		if(write(fd, str, len) != len){
			const int olderrno = errno;
			close(fd);
			errno = olderrno;
			err(6, "failed to write into file %s", file);
		}
		close(fd);
		exit(0);
	}

	int status;
	if(wait(&status) == -1)
		err(7, "could not wait for child process");

	if(!WIFEXITED(status))
		errx(8, "child process did not terminate normally");

	if(WEXITSTATUS(status))
		errx(9, "child process did not exit with status 0");
	
	ssize_t fd = open(file, O_RDONLY);
	if(fd == -1)
		err(10, "could not open file %s", file);

	uint16_t buf;
	ssize_t read_sz;
	while((read_sz = read(fd, &buf, sizeof(buf))) > 0){
		while(write(1, &buf, sizeof(buf)) != sizeof(buf)){
			const int olderrno = errno;
			close(fd);
			errno = olderrno;
			err(99, "could not write to stdout");
		}
		write(1," ", 1); // TODO: check this write
	}
	if(read_sz == -1){
		const int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(99, "could not read from file %s", file);
	}
	write(1, "\n", 1); // TODO: check this write

	exit(0);
}
