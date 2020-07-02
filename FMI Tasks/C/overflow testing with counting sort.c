// Sample use: cat /dev/urandom | head -68899 > file.bin
// ./main file.bin

#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv){
	if(argc != 2)
		errx(1, "Invalid number of arguments. Usage: %s <file.bin>", argv[0]);

	char *file = argv[1];

	struct stat st;

	if(stat(file, &st) == -1)
		err(2, "could not stat file %s", file);

	if(!(st.st_mode & S_IRUSR) || !(st.st_mode & S_IWUSR))
		errx(3, "file %s not readable or not writable", file);

	uint16_t counting[256] = { 0 }; // uint32_t for less overflows
	int16_t level[256] = { 0 };
	
	ssize_t fd = open(file, O_RDWR);
	if(fd == -1)
		err(4, "error while opening file %s", file);

	ssize_t read_sz;
	uint8_t b;	
	
	while((read_sz = read(fd, &b, sizeof(b))) == sizeof(b)){ 
		++counting[b];
		if(counting[b] == UINT16_MAX){
			counting[b] = 0;
			++level[b];
		}
	}	
	if(read_sz == -1){
		const int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(5, "error while reading file %s", file);
	}

	if(lseek(fd, 0, SEEK_SET) == -1){
		const int olderrno = errno;
		close(fd);
		errno = olderrno;
		err(6, "error while lseek file %s", file);
	}

	uint16_t i = 0;

	uint32_t overflows = 0;
	for (int j = 0; j< 256; ++j){
			overflows+=level[j];
	}
	
	for(; i < 256; ++i){
		while(level[i] >= 0){
			--level[i];
			b = i;
			while(counting[i]--){
				if(write(fd, &b, sizeof(b)) != sizeof(b)){
					const int olderrno = errno;
					close(fd);
					errno = olderrno;
					err(7, "error while writing into file %s", file);
				}
			}
			counting[i] = UINT16_MAX; // UINT32_MAX
		}
	}	
	printf("File %s is sorted inplace.\nTotal overflows: %u\n", file, overflows);
	close(fd);
	exit(0);
}
