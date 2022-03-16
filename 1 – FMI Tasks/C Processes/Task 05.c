/*
	5. Да се напише програма на C, която е аналогична на предходния пример, но принуждава процеса родител да изчака 
  	детинския си процес да завърши своето изпълнение.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define N 1000

int main(void){
	int status;
	if(fork() > 0){
		wait(&status);
		for(int i = 0; i < N; ++i)
			printf("parent\n");
	} else {
		for(int i = 0; i < N; ++i)
			printf("child\n");
	} exit(0);
}
