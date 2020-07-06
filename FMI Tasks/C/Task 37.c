// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

void clo(const int olderrno, ssize_t fd_p, ssize_t fd_f1, ssize_t fd_f2, const int errNO){
	if(fd_p != -1) close(fd_p);
	if(fd_f1 != -1) close(fd_f1);
	if(fd_f2 != -1) close(fd_f2);
	errno = olderrno;
	err(errNO, "system error occured");
}

int main(int argc, char **argv){
	if(argc != 4)
		errx(1, "Invalid number of arguments. Usage: %s <patch.bin> <f1.bin> <f2.bin>", argv[0]);

	const char *patch = argv[1], *f1 = argv[2];
	char *f2 = argv[3];

	struct stat st_p, st_f1;

	struct window{
		uint16_t displacement;
		uint8_t old;	
		uint8_t new;
	}__attribute__((packed))fmt; // no need for packing, just a reminder

	if(stat(patch, &st_p) == -1 || stat(f1, &st_f1) == -1)
		err(2, "failed to stat one or both files %s and %s", patch, f1);

	if(st_p.st_size % sizeof(fmt) != 0)
		errx(3, "file %s is corrupted", patch);

	if(!(st_p.st_mode & S_IRUSR) || !(st_f1.st_mode & S_IRUSR))
		errx(4, "one or both files %s and %s are not nreadable", patch, f1);

	ssize_t fd_p = open(patch, O_RDONLY);
	if(fd_p == -1)
		err(5, "failed to open file %s", patch);

	ssize_t fd_f1 = open(f1, O_RDONLY);
	if(fd_f1 == -1)
		clo(errno, fd_p, -1, -1, 6);
	
	ssize_t fd_f2 = open(f2, O_CREAT | O_TRUNC | O_RDWR, st_p.st_mode);
	if(fd_f2 == -1)
		clo(errno, fd_p, fd_f1, -1, 7);

	uint8_t buf[1<<10], b;
	ssize_t read_sz, write_sz;
	// copy file f1.bin to file f2.bin
	while((read_sz = read(fd_f1, &buf, sizeof(buf))) > 0)
		if((write_sz = write(fd_f2, &buf, read_sz)) != read_sz)
			clo(errno, fd_p, fd_f1, fd_f2, 8);
	if(read_sz == -1)
		clo(errno, fd_p, fd_f1, fd_f2, 9);
	// end of copy (now we have duplicated file f1.bin into file f2.bin)
	
	while((read_sz = read(fd_p, &fmt, sizeof(fmt))) == sizeof(fmt)){
		if(lseek(fd_f1, fmt.displacement, SEEK_SET) == -1)
			clo(errno, fd_p, fd_f1, fd_f2, 9);
		ssize_t read_b;
		if((read_b = read(fd_f1, &b, 1)) == 1){
			if(fmt.old == b){
				if(lseek(fd_f2, fmt.displacement, SEEK_SET) == -1)
					clo(errno, fd_p, fd_f1, fd_f2, 10);
				if((write_sz = write(fd_f2, &fmt.new, 1)) != 1)
					clo(errno, fd_p, fd_f1, fd_f2, 11);
			}
		}
		if(read_b == -1)
			clo(errno, fd_p, fd_f1, fd_f2, 12);
	}
	if(read_sz == -1)
		clo(errno, fd_p, fd_f1, fd_f2, 13);

	close(fd_p);
	close(fd_f1);
	close(fd_f2);
	exit(0);
}
