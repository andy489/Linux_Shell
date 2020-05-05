// Swap contents of two files
// github.com/andy489

#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>

void closeFree(int fd1, int fd2, void* buf){
	if(fd1 != -1) close(fd1);
	if(fd2 != -1) close(fd2);
	free(buf);
}

void errorHandler(int code, int errnum, void* buf, int fd1, int fd2){
	closeFree(fd1,fd2,buf);
	errno = errnum;
	err(code, "error occurred"); 
}

int main(int argc, char* argv[]){
	if (argc != 3) errx(1, "Invalid number of arguments");

	int fd1 = -1, fd2 = -1;

	fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1) errorHandler(2, errno, NULL, fd1, fd2);

	fd2 = open(argv[2], O_RDONLY);
	if (fd2 == -1) errorHandler(3, errno, NULL, fd1, fd2);

	struct stat st;

	if (stat(argv[1], &st) == -1)
		errorHandler(4, errno, NULL, fd1, fd2);

	void* buf = malloc(st.st_size);

	if (buf == NULL){
		closeFree(fd1,fd2,buf);
		errx(5,"Not enough memory");
	}

	if (read(fd1, buf, st.st_size) != st.st_size){
		closeFree(fd1,fd2,buf);
		errx(6,"error");
	}
	
	fd1 = open(argv[1], O_TRUNC | O_WRONLY);
	if (fd1 == -1) errorHandler(7,errno,buf,fd1,fd2);
	
	char c[4096];
	ssize_t read_size;
	
	while ((read_size = read(fd2, &c, sizeof(c))) > 0){
		if (write(fd1, &c, read_size) != read_size){
			closeFree(fd1,fd2,buf);
			errx(8,"error while writing");
		}
	}

	fd2 = open(argv[2], O_TRUNC | O_WRONLY);
	if (fd2 == -1) errorHandler(9, errno, buf, fd1, fd2);

	if (write(fd2, buf, st.st_size) != st.st_size){
		closeFree(fd1,fd2,buf);
		errx(10,"error while writing");
	}
	closeFree(fd1,fd2,buf);
	exit(0);
}
