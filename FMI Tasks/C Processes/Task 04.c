/*
	4. Да се напише програма на C, която създава процес дете и демонстрира принцина на конкурентност при процесите.
*/

// github.com/andy489
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define N 100000

int main(void){
	if(fork() > 0)
		for(int i=0; i < N; ++i)
			printf("parent\n");
	else 
		for(int i=0; i < N; ++i)
			printf("child\n");
	exit(0);
}
