/*
    11. Да се напише програма на C, която изпълнява последователно подадените ѝ като параметри команди, 
    като реализира следната функционалност постъпково:
      - main cmd1 ... cmdN Изпълнява всяка от командите в отделен дъщерен процес.
      - ... при което се запазва броя на изпълнените команди, които са дали грешка и броя на завършилите успешно.
*/

#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>

int main(int argc, char **argv){
	size_t successful = 0, failed = 0;

	for(int i = 1; i< argc; ++i){
		const char *cmd = argv[i];
		const pid_t child_pid = fork();
		if(child_pid == -1)
			err(1, "failed to fork %s", cmd);
		
		if(child_pid == 0)
			if(execlp(cmd, cmd, (char*)NULL) == -1)
				err(2,"error while execlp command %s", cmd);
				
		int status;
		if(wait(&status) == -1)
			err(2,"could not wait for current child");
		
		if(WIFEXITED(status)){
			if(WEXITSTATUS(status)) ++failed;
			else ++successful;
		} else ++failed;
	}

	printf("successful: %zu, failed: %zu\n", successful, failed);
	exit(0);
}
