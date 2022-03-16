// github.com/andy489

/*
	PROBLEM:
	65535 * sizeof(uint16_t) = 65535 * 2 = 131070 bytes (MAX)
	256 KB = 256 * 1024 = 262144 bytes RAM

	NO DRAMA, we can do it with only 1 qsort, heapsort or with counting sort.
*/

#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int cmp(const void *a, const void *b){
	if(*((uint16_t*)a) > *((uint16_t*)b))
		return 1;
	else if(*((uint16_t*)a) < *((uint16_t*)b))
		return -1;
	return 0;
}

int main(int argc, char **argv){
	if(argc != 3)
		errx(1, "Invalid number of arguments. Usage: %s <input.bin> <output.bin>", argv[0]);

	char *input = argv[1], *output = argv[2];

	struct stat st;
	
	if(stat(input, &st) == -1)
		err(2, "could not stat file %s", input);

	if(st.st_size % sizeof(uint16_t) != 0)
		errx(3, "file %s is corrupted", input);

	if(!(st.st_mode & S_IRUSR))
		errx(4, "file %s is not readable", input);

	ssize_t fd_i = open(input, O_RDONLY);
	if(fd_i == -1)
		err(5, "could not open file %s", input);

	uint8_t buf[(1<<16) * sizeof(uint16_t)];
	ssize_t read_sz = read(fd_i, &buf, sizeof(buf));
	if(read_sz == -1){
		const int olderrno = errno;
		close(fd_i);
		errno = olderrno;
		err(6, "could not read from file %s into buffer", input);	
	}
	
	if(heapsort(buf, read_sz/sizeof(uint16_t), sizeof(uint16_t), cmp) == -1){
		const int olderrno = errno;
		close(fd_i);
		errno = olderrno;
		err(8, "could not heapsort uint16_t elements in buffer");
	}
	
	ssize_t fd_o = open(output, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if(fd_o == -1){
		const int olderrno = errno;
		close(fd_i);
		errno = olderrno;
		err(9, "could not open file %s", output);
	}
	
	if(write(fd_o, &buf, read_sz) != read_sz){
		const int olderrno = errno;
		close(fd_i);
		close(fd_o);
		errno = olderrno;
		err(10, "could not write to file %s", output);
	}

	close(fd_i);
	close(fd_o);
	exit(0);
}
