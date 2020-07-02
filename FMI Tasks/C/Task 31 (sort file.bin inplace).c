#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv){
	if(argc != 2){
		errx(1, "Invalid number of arguments. Usage: %s <file.bin>", argv[0]);
	}

	char *file = argv[1];

	struct stat st;

	if(stat(argv[1], &st) == -1){
		err(2, "error while stat %s", file);
	}	

	if(!(st.st_mode & S_IRUSR) || !(st.st_mode & S_IWUSR)){
		errx(3, "file %s not readable or writable", file);
	}	

	ssize_t fd = open(file, O_RDWR);
	if(fd == -1){
		err(4, "error while opening file %s", file);
	}
	
	//printf("size of binary file: %lld\n", st.st_size);
	//printf("MAX size of uint32_t: %u\n", UINT32_MAX);

	if(st.st_size > UINT32_MAX){
		warnx("Counting sort might not work. Small chance of overflow");
	}

	uint32_t counting[256] = { 0 };

	uint8_t b;
	ssize_t read_sz;	
	
	while((read_sz = read(fd, &b, sizeof(b))) == sizeof(b)){
		++counting[b];
	}
	if(read_sz == -1){
		const int olderrno = errno;
		close(fd);	
		errno = olderrno;
		err(5, "error while reading file %s", file);
	}
	
	ssize_t ls = lseek(fd, 0, SEEK_SET);
	if(ls == -1){
		const int olderrno = errno;
		close(fd);	
		errno = olderrno;
		err(6, "error while lseek file %s", file);
	}

	uint16_t i = 0;
	for(; i < 256; ++i){
		b = i;
		while(counting[i]){
			if( write(fd, &b, sizeof(b)) != sizeof(b) ){
				const int olderrno = errno;
				close(fd);	
				errno = olderrno;
				err(7, "error while rewriting file %s", file);
			}
			--counting[i];
		}
	}
	
	close(fd);
	exit(0);
}
