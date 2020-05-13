// github.com/andy489

#include <stdio.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#define SIZE 256

int main(int argc, char** argv){
	if(argc != 3){
		errx(1,"Usage: %s <file.bin> <sorted.bin>", argv[0]);
	}	
	char * file = argv[1], * sorted = argv[2];

	int fd1, fd2;
	fd1 = open(file, O_RDWR);
	if(fd1 == -1){
		err(2, "error while opening file %s", file);
	}	
	
	fd2 = open(sorted, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd2 == -1){
		int _errno=errno;
		close(fd1);
		errno=_errno;
		err(3, "error while opening file %s", sorted);
	}	

	const uint32_t BUF_SIZE = 2<<10;
	uint8_t histo[SIZE] = { 0 };
	uint8_t buf[BUF_SIZE];
	ssize_t read_sz;
	uint32_t total_sz = 0;
     
	while((read_sz = read(fd1, &buf, sizeof(buf))) > 0){
		for(int i = 0; i< read_sz; ++i){
			++histo[buf[i]];
			++total_sz;
		}
	}
     
	if(read_sz == -1){
		int _errno=errno;
		close(fd1); close(fd2);
		errno=_errno;
		err(3, "error while reading from file %s", file);
	}
	// reset marker to the beginning of the file for inplace sorting
	// lseek(fd1, 0, SEEK_SET);
	uint16_t byte = 0;
	// while the counts are not fully traversed
	while (byte < SIZE){
		// fill the buffer for writing
		size_t pos = 0;
		while(pos < BUF_SIZE && byte < SIZE){
			if(histo[byte] == 0){
				++byte; // move to next byte
			} 
			else {
				buf[pos] = byte; // write the byte inside the buffer
				--histo[byte]; // decrement counter for this byte
				++pos; // move one step forward in the buffer
			}
		}
		ssize_t write_sz = pos;
		if(write(fd2, buf, write_sz) != write_sz){ // for inplace sorting write to fd1
			int _errno=errno;
			close(fd1); close(fd2);
			errno=_errno;
			err(4, "error while writing to file %s", sorted);
		}
	}
	close(fd1); close(fd2);
	exit(0);
}
