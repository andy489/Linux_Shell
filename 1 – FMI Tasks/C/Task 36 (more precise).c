#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

void READ(int fd, const char *from, uint8_t numerate, uint16_t *cnt){
	char c;
	ssize_t read_sz;

	int newline = 1;

	while((read_sz = read(fd, &c, 1)) == 1){
		if(numerate){
			if(newline){
				setbuf(stdout, NULL);
				fprintf(stdout, "%d ", *cnt);
				write(1, &c, 1);
				newline = 0;
				++(*cnt);
			} else write(1, &c, 1);
		if(c == '\n') newline = 1;
		} else write (1,&c, 1);
	}
	if(read_sz == -1){
		const int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(99, "error while reding from %s", from);
	}
}

int main(int argc, char **argv){
	uint16_t i = 1, cnt = 1;
	uint8_t numerate = 0;
	
	if(argc == 1){
		READ(0, "STDIN", numerate, &cnt);
		exit(0);
	} else if (argc == 2 && strcmp(argv[1], "-n") == 0){
		numerate = 1;
		READ(0, "STDIN", numerate, &cnt);
		exit(0);
	}
	
	if(strcmp(argv[i], "-n") == 0){
		numerate = 1;
		++i;
	}

	for(; i< argc; ++i){
		if(strcmp("-", argv[i]) == 0){
			READ(0, "STDIN", numerate, &cnt);
			continue;
		}

		const char *filepath = argv[i];

		struct stat st;		
		if(stat(filepath, &st) == -1)
			err(1, "could not stat file %s", filepath);
		
		if(!(st.st_mode & S_IRUSR)){
			warnx("file %s is not readable", filepath);
			continue;
		}
	
		ssize_t fd = open(filepath, O_RDONLY);
		if(fd == -1)
			err(3, "failed to open file %s", filepath);

		READ(fd, filepath, numerate, &cnt);
	}
	exit(0);
}
