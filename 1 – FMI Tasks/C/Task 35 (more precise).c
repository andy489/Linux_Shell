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
	if(argc != 4)
		errx(1, "Invalid number of arguments. Usage: %s <f1.bin> <f2.bin> <patch.bin>", argv[0]);

	char *f1 = argv[1], *f2 = argv[2], *patch = argv[3];

	struct stat st1, st2;
	if(stat(f1, &st1) == -1 || stat(f2, &st2) == -1)
		err(2, "Could not stat one or both files %s and %s", f1, f2);

	if(!(st1.st_mode & S_IRUSR) || !(st2.st_mode & S_IRUSR))
		errx(3, "One or both files %s and %s is not readable", f1, f2);

	if(st1.st_size != st2.st_size || st1.st_size > UINT16_MAX)
		errx(4, "input files are corrupted");

	struct window{
		uint16_t displacement;
		uint8_t old;
		uint8_t new;
	}__attribute__((packed))fmt; // no need to pack, but just as a reminder

	ssize_t fd1 = open(f1, O_RDONLY);
	if(fd1 == -1)
		err(5, "error while opening file %s", f1);

	ssize_t fd2 = open(f2, O_RDONLY);
	if(fd2 == -1){
		const int olderrno = errno;
		close(fd1);
		errno = olderrno;
		err(6, "error while opening file %s", f2);
	}

	ssize_t fd3 = open(patch, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if(fd3 == -1){
		const int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(7, "error while opening file %s", patch);
	}

	uint8_t b1, b2;
	uint16_t cnt = 0;
	ssize_t read_sz1, read_sz2;
	while(((read_sz1 = read(fd1, &b1, sizeof(b1))) == sizeof(b1)) && ((read_sz2 = read(fd2, &b2, sizeof(b2))) == sizeof(b2))){
		if(b1 != b2){
			fmt.displacement = cnt;
			fmt.old = b1;
			fmt.new = b2;

			ssize_t wr;
			if((wr = write(fd3, &fmt, sizeof(fmt))) != sizeof(fmt)){
				const int olderrno = errno;
				close(fd1);
				close(fd2);
				close(fd3);
				errno = olderrno;
				err(8, "failed to write to file %s", patch);
			}
		}
		++cnt;
	}
	if(read_sz1 == -1 || read_sz2 == -1){
		const int olderrno = errno;
		close(fd1);
		close(fd2);
		errno = olderrno;
		err(9, "failed to read from file %s or %s", f1, f2);
	}

	close(fd1);
	close(fd2);
	close(fd3);
	exit(0);
}
