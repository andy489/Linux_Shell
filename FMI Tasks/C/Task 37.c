// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int main(int argc, char **argv){
	if(argc != 4){
		errx(1,"Usage: %s <patch.bin> <f1.bin> <f2.bin>", argv[0]);
	}

	const char *patch = argv[1], *f1 = argv[2];
	char *f2 = argv[3];

	struct tripe_t{
		uint16_t displacement;
		uint8_t old;
		uint8_t new;
	} fmt;

	struct stat st_p;

	if(stat(patch, &st_p) == -1){
		err(2,"error while stat %s", patch);
	}

	off_t p_sz = st_p.st_size;
	
	if(p_sz % sizeof(fmt) !=0){
		err(3, "%s file is corrupted", patch);
	}

	ssize_t fd1 = -1;
	if( (fd1 = open(f1, O_RDONLY)) == -1 ){
		err(4,"error while opening file %s", f1);
	}

	ssize_t fd2 = -1;
	if((fd2 = open(f2, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP)) == -1){
		const int _errno = errno;
		close(fd1);
		errno = _errno;
		err(5,"error while opening file %s", f2);
	}
	
	uint8_t buf[2<<10];
	ssize_t rd = -1;
	while((rd = read(fd1, &buf, sizeof(buf)))> 0){
		int wr = write(fd2, &buf, rd);
		if(wr == -1 || wr != rd){
			const int _errno = errno;
			close(fd1);
			close(fd2);
			errno = _errno;
			err(6,"error while writing to file %s", f2);
		}
	}
	if(rd == -1){
		const int _errno = errno;
		close(fd1);
		close(fd2);
		errno = _errno;
		err(7,"err while reading from file %s", f1);
	}
	
	ssize_t fd_p = -1;
	if((fd_p = open(patch, O_RDONLY)) == -1){
		const int _errno = errno;
		close(fd1);
		close(fd2);
		errno = _errno;
		err(8,"err while opening file %s", patch);
	}
	
	// now we have duplicated the file f1 into f2

	rd = -1;
	ssize_t sz = sizeof(fmt);
	while((rd = read(fd_p, &fmt, sz)) == sz){
		ssize_t ls = lseek(fd2, fmt.displacement, SEEK_SET);
		if(ls == -1){
			const int _errno = errno;
			close(fd1);
			close(fd2);
			close(fd_p);
			errno = _errno;
			err(9,"err while lseek file %s", f2);
		}
		ssize_t wr = write(fd2, &fmt.new, sizeof(fmt.new));
		if(wr == -1){
			const int _errno = errno;
			close(fd1);
			close(fd2);
			close(fd_p);
			errno = _errno;
			err(10,"err while writing to file %s", f2);
		}
	}
	if(rd == -1){
		const int _errno = errno;
		close(fd1);
		close(fd2);
		close(fd_p);
		errno = _errno;
		err(11,"err while reading from file %s", patch);
	}	
	close(fd1);
	close(fd2);
	close(fd_p);
	exit(0);
}
