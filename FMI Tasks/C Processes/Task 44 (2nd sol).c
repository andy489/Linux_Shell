// github.com/andy489
// second solution
#include <err.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(){
    char buf[1<<8];
    int read_size;

	 const char *prompt = "andy's simple prompt: ";
	 const size_t len = strlen(prompt);

    write(1, prompt, len);

    while((read_size = read(0, buf, sizeof(buf))) > 0){
        char cmd[1<<8];
        int i = 0;
        for(; i < read_size; ++i){
            if(buf[i] == ' ' || buf[i] == '\n' || buf[i] == '\t')
                break;
            cmd[i] = buf[i];
        }
        cmd[i]='\0';

        if(strlen(cmd) == 0)
            break;

        if(strcmp(cmd, "exit") == 0)
            break;

        pid_t child_pid = fork();
        if(child_pid == -1)
            err(1, "could not fork");

        if(child_pid == 0){
            if(execlp(cmd, cmd, (char*)NULL) == -1)
                err(2, "could not exec: %s", cmd);
        }

        if(wait(NULL) == -1)
            err(3, "could not wait");
        write(1, prompt, len);
    }
    exit(0);
}
