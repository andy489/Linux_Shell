#include <err.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
	if(argc != 2)
		err(1, "Invalid number of arguments. Usage: %s <filepath>", argv[0]);

	const char *file = argv[1];

	if(mkfifo("mypipe", 0600) == -1)
		err(2, "could not create named pipe");

	int fdp = open("mypipe", O_WRONLY);
	if(fdp == -1)
		err(3, "could not open named pipe");

	if(dup2(fdp, 1) == -1)
		err(4, "could not dup2");

	if(execlp("cat", "cat", file, (char *)NULL) == -1)
		err(5, "could not execlp cat");
}
