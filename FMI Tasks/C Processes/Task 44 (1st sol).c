// github.com/andy489
// 1st solution

#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>

int main(void){
	while(1){
		if(write(1, "Pesho's simple prompt: ", 23) != 23)
			err(1, "could not write the prompt name to stdout");

	char cmd[1<<5];
	ssize_t read_sz = read(0, &cmd, sizeof(cmd));
	if(read_sz == -1)
		err(2, "could not read from stdin");

	//changing newline to terminating zero '\n' -> '\0':
	cmd[read_sz - 1] = '\0';

	if(strcmp(cmd, "exit") == 0)
		break;

	const char *bin = "/bin/";
	char buf[1<<6];
	strcpy(buf,bin); // copy /bin/
	strcat(buf,cmd); // append cmd

	const pid_t child = fork();
	if(child == -1){
		warn("could not fork for %s", buf);
		continue;
	}

	if(child == 0) // we are in child for exec cmd in /bin
		if(execl(buf, "Pesho's process", (char*)NULL) == -1)
			err(3, "could not execl %s", buf);
	
	if(wait(NULL) == -1)
		err(4, "could not wait %s", buf);
	}
	exit(0);
}
