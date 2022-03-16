/*
	2. Да се напише програма на C, която изпълнява команда ls с точно един аргумент.
*/

#include <err.h>
#include <unistd.h>

int main(int argc, char **argv){
	if(argc != 2){
		errx(1, "Invalid number of arguments. Usage: %s <dirpath>", argv[0]);
	}
		
	const char *cmd = "ls", *dirpath = argv[1];	

	if(execlp(cmd, "Pesho is listing a directory content", dirpath, 0) == -1)
		err(2,"could not execlp ls");
}
