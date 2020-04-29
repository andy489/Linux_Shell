#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include<stdio.h>

int main(int argc, char** argv){
	if(argc <3){
		write(2, "Invalid number of arguments!\n",29);
	}
	exit(renameat2(AT_FDCWD,argv[1],AT_FDCWD,argv[2], RENAME_EXCHANGE));
}
