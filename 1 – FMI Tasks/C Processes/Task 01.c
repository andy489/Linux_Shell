/*
	1. Да се напише програма на C, която изпълнява команда date.
*/

#include <unistd.h>
#include <err.h>
#include <stdio.h>

int main(){
	const char * cmd = "date";
	if(execl("/bin/date", "date", "+%D", (char *)NULL) == -1)
		err(1,"failed to execl %s", cmd);
	else 
		printf("this will never be printed\n");				
}
