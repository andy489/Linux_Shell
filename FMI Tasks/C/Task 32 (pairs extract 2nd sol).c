// github.com/andy489

#include <stdlib.h>
#include <sys/types.h>
#include <err.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void clo(ssize_t fd1, ssize_t fd2, ssize_t fd3){
	if(fd1 != -1) close(fd1);
	if(fd2 != -1) close(fd2);
	if(fd3 != -1) close(fd3);
}

int main(int argc, char **argv){
	if(argc != 4)	
		errx(1, "Invalid number of arguments. Usage: %s <f1> <f2> <f3>", argv[0]);

	char *f1 =argv[1], *f2 = argv[2], *f3 = argv[3];
	
	struct stat st1;
	struct stat st2;
	struct stat st3;

	if(stat(f1, &st1) == -1)
		err(2, "error while stat %s", f1);

	if(stat(f2, &st2) == -1)
		err(3, "error while stat %s", f2);

	if(stat(f3, &st3) == -1)
		err(4, "error while stat %s", f3);

	if(!(st1.st_mode & S_IRUSR) || !(st2.st_mode & S_IRUSR) || !(st3.st_mode & S_IWUSR))
		errx(5, "%s is not readable, or %s is not readable, or %s is not writable", f1, f2, f3);

	struct window {
		uint32_t x;
		uint32_t y;
	}__attribute__((packed))fmt; //not needed in this case, but for reminder

	if((st1.st_size % sizeof(fmt) !=0) || (st2.st_size % sizeof(uint32_t) != 0))
		errx(6, "one or both files %s and %s is/are corrupted", f1, f2);

	ssize_t fd1 = -1, fd2 = -1, fd3 = -1;
	
	fd1 = open(f1, O_RDONLY);
	if(fd1 == -1)
		err(7, "error while opening file %s", f1);

	fd2 = open(f2, O_RDONLY);
	if(fd2 == -1){
		const int olderrno = errno;
		clo(fd1, fd2, fd3);
		errno = olderrno;
		err(8, "error while opening file %s", f2);
	}
		
	fd3 = open(f3, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd3 == -1){
		const int olderrno = errno;
		clo(fd1, fd2, fd3);
		errno = olderrno;
		err(9, "error while opening file %s", f3);
	}

	ssize_t rd1 = -1;
	while((rd1 = read(fd1, &fmt, sizeof(fmt))) > 0){
		uint32_t from = fmt.x;
		uint32_t len = fmt.y;
		off_t ls = lseek(fd2, (from - 1) * sizeof(uint32_t), SEEK_SET);
		if(ls == -1){
			const int olderrno = errno;
			clo(fd1, fd2, fd3);
			errno = olderrno;
			err(10, "error while lseek file %s", f2);
		}
		uint32_t buf;
		uint32_t cnt = 0;
		ssize_t rd2 = -1;
		while((rd2 = read(fd2, &buf, sizeof(buf))) > 0){
			++cnt;
			ssize_t wr = write(fd3, &buf, rd2);
			if(wr != rd2){
				const int olderrno = errno;
				clo(fd1, fd2, fd3);
				errno = olderrno;
				err(11, "error while writing to file %s", f3);
			}
			if (cnt == len) break;
		}
		if(rd2 == -1){
			const int olderrno = errno;
			clo(fd1, fd2, fd3);
			errno = olderrno;
			err(12, "error while reading file %s", f2);
		}
	}
	if(rd1 == -1){
		const int olderrno = errno;
		clo(fd1, fd2, fd3);
		errno = olderrno;
		err(13, "error while reading file %s", f1);
	}
	exit(0);
}
