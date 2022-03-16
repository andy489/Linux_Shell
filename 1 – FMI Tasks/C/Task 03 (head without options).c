// Implement a head function withoud options
// github.com/andy489

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

int main(int argc, char* argv[]){
	int fd, lines = 0;
	char c;

	if (argc != 2) {
		errx(1, "Invalid number of arguments");
	}

	const char *file = argv[1];

	if ((fd = open(file, O_RDONLY)) == -1) {
		err(2, "failed to open %s", file);
	}
	
	int rd;
	while ((rd = read(fd, &c, 1)) > 0) {
		if(rd == -1){
			int _errno=errno;
			close(fd);
			errno=_errno;
			err(3, "failed to read %s", file);
		}
		if (c == '\n') ++lines;
		write(1, &c, 1);
		if (lines == 10) break;
	}
	close(fd);
	exit(0);
}
