// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, char **argv){
	if(argc != 4){
		errx(1,"Usage: %s <f1.bin> <f2.bin> <patch.bin>", argv[0]);
	}
	
	const char *f1 = argv[1], *f2 = argv[2];
	char *patch = argv[3];

	struct stat st1;
	struct stat st2;

	if(stat(f1, &st1) == -1){
		err(2,"error while stat file %s", f1);
	}

	if(stat(f2, &st2) == -1){
		err(3,"error while stat file %s", f2);
	}

	off_t sz1 = st1.st_size, sz2 = st2.st_size;

	if(sz1 != sz2){
		errx(4,"files %s and %s are not consistеnt", f1, f2);
	}
	
	struct triple_t{
		uint16_t displacement;
		uint8_t old;
		uint8_t new;
	}fmt;
	
	ssize_t fd1 = -1, fd2 = -1;

	if((fd1 = open(f1, O_RDONLY)) == -1){
		err(5,"error while opening file %s", f1);
	}
	
	if((fd2 = open(f2, O_RDONLY)) == -1){
		const int _errno = errno;
		close(fd1);
		errno = _errno;
		err(6,"error while opening file %s", f2);
	}
	
	ssize_t fd_p = open(patch, O_CREAT|O_TRUNC|O_RDWR ,S_IRUSR|S_IWUSR|S_IRGRP);
   if(fd_p == -1){
		const int _errno = errno;
		close(fd1);
		close(fd2);
		errno = _errno;
      err(7,"error while opening file %s", patch);
   }

	ssize_t rd1 = -1, rd2 = -1;
	uint8_t buf1, buf2;
	uint16_t cnt = 0x0; 
	while((rd1 = read(fd1, &buf1, 1)) > 0 && (rd2 = read(fd2, &buf2, 1)) > 0 ){
		if(buf1 != buf2){
			fmt.displacement = cnt;
			fmt.old = buf1;
			fmt.new = buf2;
			ssize_t wr = write(fd_p, &fmt, sizeof(fmt));
			if(wr == -1 || wr != sizeof(fmt)){
				const int _errno = errno;
				close(fd1);
				close(fd2);
				close(fd_p);
				errno = _errno;
				err(8,"error while writing to file %s", patch);
			}
		}
		++cnt; 
	}
	if(rd1 == -1 || rd2 == -1){
		const int _errno = errno;
		close(fd1);
		close(fd2);
		close(fd_p);
		errno = _errno;
		err(9,"error while reading from files %s and %s", f1, f2);
	} 
	close(fd1);
	close(fd2);
	close(fd_p);
	exit(0);
}
