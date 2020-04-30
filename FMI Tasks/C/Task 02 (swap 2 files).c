// Swap contents of two files
// github.com/andy489

#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <err.h>

int main(int argc, char *argv[]){
	if(argc != 3) {
		errx(1,"Wrong number of arguments");
	}
	int ex;
	if((ex = renameat2(AT_FDCWD, argv[1], AT_FDCWD, argv[2], RENAME_EXCHANGE)) == -1) {
		err(2,"Fail");
	}
	return 0;
}
