// github.com/andy489

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <errno.h>
#include <unistd.h>

int main(){
	char* etc = "/etc/passwd";
	char* new = "./passwd.txt";

	int fd1 = open(etc, O_RDONLY);
	if(fd1 == -1){
		err(1,"error while opening %s", etc);		
	}
	
	int fd2 = open(new, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if(fd2 == -1){
		int _errno=errno;
		close(fd1);
		errno=_errno;
		err(2,"error while opening %s", new);
	}
	
	char c;
	
	while(read(fd1, &c, 1)){
		if(c == ':'){
			c = '?';
		}
		if(write(fd2, &c, 1) != 1){
			int _errno=errno;
			close(fd1);
			close(fd2);
			errno=_errno;
			err(3,"error while writing %s", new);
		}
	}
	close(fd1);
	close(fd2);
	exit(0);
}
