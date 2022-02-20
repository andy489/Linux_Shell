/*
	3. Да се напише програма на C, която изпълнява команда sleep (за 60 секунди).
*/

#include <err.h>
#include <unistd.h>

int main(void){
	const char *cmd = "sleep", *arg = "60";

	if(execl(cmd, "Pesho sleeps for 60 sec", arg, (char*)NULL) == -1)
		err(1, "could not execl %s", cmd);
}
