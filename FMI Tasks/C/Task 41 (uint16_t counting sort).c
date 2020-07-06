// github.com/andy489

/*
	PROBLEM:
	65535 * sizeof(uint16_t) = 65535 * 2 = 131070 bytes (MAX)
	256 KB = 256 * 1024 = 262144 bytes RAM

	NO DRAMA, we can do it with only 1 qsort, ot with counting sort.
*/

#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

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


	uint32_t counting[0xffff + 1] = { 0 };

	uint16_t buf[1<<10];
	ssize_t read_sz;
	while((read_sz = read(fd_i, &buf, sizeof(buf))) > 0){
		for(uint16_t i = 0; i< read_sz / sizeof(uint16_t); ++i)
			++counting[buf[i]];
	}	
	if(read_sz == -1){
		const int olderrno = errno;
		close(fd_i);
		errno = olderrno;
		err(6, "could not read from file %s", input);
	}

	ssize_t fd_o = open(output, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if(fd_o == -1){
		const int olderrno = errno;
		close(fd_i);
		errno = olderrno;
		err(7, "could not open file %s", output);
	}
	/* 
		If the counter i is of type uint16_t, it will overfloat at the last incrementation
		of the cycle. That will lead us to an endless loop. So make sure that the counter
		can be at least 0xffff + 1 or brake it at the last iteration;
	*/
	for(uint32_t i = 0; i<= 0xffff; ++i){
		while(counting[i]--){
			if(write(fd_o, &i, sizeof(uint16_t)) != sizeof(uint16_t)){
				const int olderrno = errno;
				close(fd_i);
				close(fd_o);
				errno = olderrno;
				err(8, "could not write to file %s", output);
			}
		}
		// if(i == 0xffff) break;
	}

	close(fd_i);
	close(fd_o);
	exit(0);
}
