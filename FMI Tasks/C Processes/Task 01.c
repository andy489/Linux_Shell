/*
	Да се напише програма на C, която изпълнява команда date.
*/

// github.com/andy489
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>

int main(){
	const char * cmd = "date";
	if(execl("/bin/date", "date", "+%D", (char *)NULL) == -1){
		err(1,"failed to execl %s", cmd);
	} else {
		printf("this will never be printed\n");		
	}		
	exit(0);
}
