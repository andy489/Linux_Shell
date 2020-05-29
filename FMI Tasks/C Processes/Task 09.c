/*
    9. Да се напише програма на C, която която създава файл в текущата директория и генерира два процесa, 
    които записват низовете foo и bar в създадения файл.
    Програмата не гарантира последователното записване на низове.
    Променете програмата така, че да записва низовете последователно, като първия е foo.
*/

// github.com/andy489
#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void err_handler(int exit_code, const char *msg, int errnum, ssize_t fd){
	if(fd != -1) close(fd);
	errno = errnum;
	err(exit_code, "error while %s", msg);
}

int main(){
	ssize_t fd = open("foobar.txt", O_RDWR|O_TRUNC|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP);
	if(fd == -1){
		err(1,"error while opening file foobar.txt");
	}	
	close(fd);	

	pid_t child_pid1 = fork();
	if(child_pid1 == -1){
		err_handler(2, "fork in first child", errno, -1);
	}

	if(child_pid1 == 0){
		// we are in first child process
		ssize_t fd1 = open("foobar.txt", O_WRONLY|O_APPEND);
		if(fd1 == -1){
			err_handler(3, "opening foobar.txt in first child", errno, -1);
		}
		if(write(fd, "foo\n", 4) != 4){
			err_handler(4, "writing into foobar.txt in first child", errno, fd1);
		}
		close(fd1);
		exit(0);
	}

	if(wait(NULL) == -1) err_handler(5, "waiting first child", errno, -1); // uncomment to order child processes
	
	const pid_t child_pid2 = fork();
	if(child_pid2 == -1){
		err_handler(4, "fork second child", errno, -1);
	}

	if(child_pid2 == 0){
		// we are in second child process
		ssize_t fd2 = open("foobar.txt", O_WRONLY|O_APPEND);
		if(fd2 == -1){
			err_handler(6, "opening foobar.txt in second child", errno, fd2);
		}	
		if(write(fd2, "bar\n", 4) != 4){
			err_handler(7, "writing into foobar.txt in second child", errno, fd2);
		}
		close(fd2);
		exit(0);
	}
	
	if(wait(NULL) == -1){
		err(8,"could not wait all children processes");
	}
	
	exit(0);
}
