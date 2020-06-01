// github.com/andy489
// first solution

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(){
	while(1){
		if(write(1, "pesho's simple prompt: ", 23 ) == -1){
			err(1, "could not write to stdout");
		}

		char buf[1<<8];	
		ssize_t bytes_read = read(0, &buf, sizeof(buf));
		if(bytes_read == -1){
			err(2, "could not read from stdin");
		}
		// change newline to terminating zero
		buf[bytes_read - 1] = '\0';

		if(strcmp(buf, "exit") == 0){
			break;
		}

		const pid_t child = fork();
		if(child == -1){
			warn("could not fork");
			continue;
		} 
		
		if(child == 0){
			// we are in child process
			if(execlp(buf, "pesho's process", (char *)NULL) == -1){
				err(3, "could not exec");
			}
		}

		if(wait(NULL) == -1){
			err(4, "could not wait");
		}
	}
	exit(0);
}
