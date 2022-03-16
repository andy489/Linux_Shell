// github.com/andy489

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <err.h>
#include <stdlib.h>

int main(int argc, char** argv){
	if(argc == 1) errx(1, "expect arguments");

	int i;
	for(i = 1; i < argc; ++i){
		int fd = open(argv[i], O_RDONLY);
		if(fd == -1){
			printf("permissions denied: %s\n", argv[i]);
			continue;
		}
		
		char c[4096];
		ssize_t read_size;
	
		while((read_size = read(fd, &c, sizeof(c))) > 0)
			write(1, &c, read_size);
		
		close(fd);
	}
	exit(0);
}
