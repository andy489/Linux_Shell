// Swap contents of two files
// github.com/andy489

#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv){
	exit(renameat2(AT_FDCWD,argv[1],AT_FDCWD,argv[2], RENAME_EXCHANGE));
}
