// Implement a head function withoud options
// github.com/andy489

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>

int main(int argc, char* argv[]){
	int fd1,
	    i = 0;
	char c;

	if (argc != 2) {
		errx(1, "Invalid number of arguments");
	}

	if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
		err(1, "File failed to open in read mode");
	}

	while (read(fd1, &c, 1)) {
		if (c == '\n') ++i;
		write(1, &c, 1);
		if (i == 10) break;
	}

	close(fd1);
	exit(0);
}
