// github.com/andy489
// 2nd solution

#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <string.h>
#include <sys/wait.h>

int main(void){
    	char buf[1<<6];
    	ssize_t read_sz;
	
	const char *prompt = "Stamat's simple prompt: ";
	const ssize_t len = (ssize_t)strlen(prompt);

    	if(write(1, prompt, len) != len)
		err(1, "could not write prompt to stdout");

    	while((read_sz = read(0, buf, sizeof(buf))) > 0){
        	char cmd[1<<6];
        	ssize_t i = 0;
        	for(; i < read_sz; ++i){
            		if(buf[i] == ' ' || buf[i] == '\n' || buf[i] == '\t')
                		break;
           	cmd[i] = buf[i];
        }
        cmd[i]='\0';

        if(strlen(cmd) == 0 || strcmp(cmd, "exit") == 0)
		break;

        const pid_t child = fork();
        if(child == -1)
		err(2, "could not fork");

        if(child == 0) // we are in child process for exec cmd
            	if(execlp(cmd, cmd, (char*)NULL) == -1)
                	err(3, "could not execlp: %s", cmd);

        if(wait(NULL) == -1)
		err(4, "could not wait");
	    
        if(write(1, prompt, len) != len)
		err(5, "could not write ptompt to stdout");
    }
    exit(0);
}
