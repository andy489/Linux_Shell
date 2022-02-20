/*
    T2 - Напишете програма на C, която демонстрира комуникация през pipe между parent и child процеси. 
    Parent-ът трябва да изпраща стринга, получен като първи аргумент на командния ред към child-а, който 
    да го отпечатва на стандартния изход.
*/

#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv){
	if(argc != 2)
		errx(1, "Invalid number of arguments. Usage %s <string>", argv[0]);

	const char *str = argv[1];
	ssize_t len = (ssize_t)strlen(str);
	
	int p[2];
	if(pipe(p) == -1)
		err(2, "could not create pipe");

	const pid_t child_pid = fork();
	if(child_pid == -1)
		err(3, "could not fork child process");

	if(child_pid == 0){ // we are in child process
		close(p[1]);
		
		if(read(p[0], &str, len) == len){
			if(write(1, &str, len) != len)
				err(4, "could not write to stdout");
		} else
			err(5,"error while reading in child process");
			
		close(p[0]);
		exit(0);
	}
	
	close(p[0]);
	if(write(p[1], str, len) != len)
		err(6, "error while writing in parent");
	
	close(p[1]);
	wait(NULL);
	exit(0);
}
