/*
	2. Да се напише програма на C, която изпълнява команда ls с точно един аргумент.
*/

// github.com/andy489
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char **argv){
	if(argc != 2){
		errx(1,"Invalid number of arguments. Usage: %s <filename>", argv[0]);
	}

	const char *filename = argv[1];	

	if(execlp("ls", "ls", filename, 0) == -1){
		err(2,"could not execlp ls");
	}
	
	exit(0);
}
