// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

void errhndl(int exitcode, int errnum, int fd1, int fd2, const char* msg){
	if(fd1 != -1) close(fd1);
	if(fd2 != -1) close(fd2);
	errno = errnum;
	err(exitcode, "%s", msg);
}

int main(int argc, char** argv){
	if(argc != 5 && argc != 3)
		errx(1, "Invalid number of arguments. Usage: %s [-d delimiter] <f1> <f2>", argv[0]);

	char* d = "\t";
	int start = 1;
	if(strcmp(argv[1], "-d") == 0){
		start = 3;
		d = argv[2];
	}

	const char *f1 = argv[start], *f2 = argv[start + 1];

	struct stat st;
	if(stat(argv[start], &st) == -1)
		err(2, "could not stat %s", f1);

	if(!(S_ISREG(st.st_mode)))
		errx(3, "%s is not a regular file", f1);

	if(!(st.st_mode & S_IRUSR))
		errx(4, "%s is not readable", f1);

	if(stat(argv[start + 1], &st) == -1)
		err(5, "could not stat %s", f2);

	if(!(S_ISREG(st.st_mode)))
		errx(6, "%s is not a regular file", f2);

	if(!(st.st_mode & S_IRUSR))
		errx(7, "%s is not redable", f2);

	int fd1 = open(argv[start], O_RDONLY);
	if(fd1 == -1)
		errhndl(8, errno, fd1, -1, "could not open 1st file");

	int fd2 = open(argv[start+1], O_RDONLY);
	if(fd2 == -1)
		errhndl(9, errno, fd1, fd2, "could not open 2nd file");

	while(1){
		int indx = 0;
		char buf[1<<10], l[1<<8], r[1<<8], c;
		ssize_t read_sz;
		while((read_sz = read(fd1, &c, 1)) > 0 && c != '\n')
			l[indx++] = c;
		if (read_sz == -1)
			errhndl(10, errno, fd1, fd2, "could not read from 1st file");

		l[indx] = '\0';
		indx = 0;

		while((read_sz = read(fd2, &c, 1)) > 0 && c != '\n')
			r[indx++] = c;
		if(read_sz == -1)
			errhndl(11, errno, fd1, fd2, "could not read from 2nd file");

		r[indx] = '\0';
		indx = 0;
		
		if(!strlen(l) && !strlen(r))
			break;

		sprintf(buf, "%s%s%s\n", l, d, r); 	
		write(1, &buf, strlen(buf));
	}	

	close(fd1);
	close(fd2);
	exit(0);
}
