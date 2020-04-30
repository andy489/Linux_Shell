// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int cmp(const void* a, const void* b){
	return *((unsigned char*)a) - *((unsigned char*)b);
}

int main(int argc, char** argv){
	if(argc != 3){
		errx(1,"expect two argument");
	}

	int fd = open(argv[1], O_RDONLY);

	if(fd < 0){
		err(2, "error opening file %s", argv[1]);
	}

	struct stat st;

	if(stat(argv[1], &st) == -1){
		err(3,"error stat file %s", argv[1]);
	}

	int fd_res = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	
	if(fd_res < 0){
		err(4, "error opening file %s", argv[2]);
	}
	
	if(!st.st_size){
		close(fd);
		close(fd_res);
		exit(0);
	}
	
	void *buf = malloc(st.st_size);

	if(!buf){
		int olderrno = errno;
		close(fd);
		close(fd_res);
		errno = olderrno;
		err(5, "no memory");
	}

	if(read(fd, buf, st.st_size) != st.st_size){
		int olderrno = errno;
		free(buf);
		close(fd);
		close(fd_res);
		errno = olderrno;
		err(6, "error while reading file %s", argv[1]);
	}
	
	qsort(buf, st.st_size, 1, cmp);
	
	if(write(fd_res, buf, st.st_size) != st.st_size){
		int olderrno = errno;
		free(buf);
		close(fd);
		close(fd_res);
		errno = olderrno;
		err(7, "error while writing file %s", argv[2]);
	}
		
	free(buf);
	close(fd);
	close(fd_res);
	exit(0);
}
