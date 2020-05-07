// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv){
	if(argc != 3) errx(1,"usage: %s file1.txt file2.txt", argv[0]);
	
	int fd1 = -1, fd2 = -1;

	const char *f1 = argv[1], *f2 = argv[2]; 

	if((fd1 = open(f1, O_RDONLY)) == -1){
		err(2,"error while opening %s", f1);
	}

	if((fd2 = open(f2, O_APPEND | O_WRONLY | O_EXCL)) == -1){
		int _errno=errno;
		close(fd1);
		errno=_errno;
		err(3,"error while opening %s", f2);
	}	

	char c[2<<11];
	int read_sz = -1;

	while((read_sz=read(fd1, &c, sizeof(c))) > 0){
		if(read_sz == -1){
			int _errno=errno;
			close(fd1);
			close(fd2);
			errno=_errno;
			err(4,"error while reading %s", f1);
		}
		int wr = write(fd2, &c, read_sz);
		if(wr == -1){
			int _errno=errno;
			close(fd1);
			close(fd2);
			errno=_errno;
			err(5,"error while writing %s", f2);
		}
	}
	close(fd1);
	close(fd2);	
	exit(0);
}
