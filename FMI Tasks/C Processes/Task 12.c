/*
    12. Да се напише програма на C, която получава като параметри от команден ред две команди (без параметри) 
    и име на файл в текущата директория. Ако файлът не съществува, го създава. 
    Програмата изпълнява командите последователно, по реда на подаването им. Ако първата команда завърши успешно, 
    програмата добавя нейното име към съдържанието на файла, подаден като команден параметър. 
    Ако командата завърши неуспешно, програмата уведомява потребителя чрез подходящо съобщение.
*/    

// github.com/andy489
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <errno.h>

int main(int argc, char **argv){
	if(argc != 3)
		errx(1, "Invalid number of arguments. Usage %s <cmd1> <cmd2>", argv[0]);

	const char *cmd1 = argv[1], *cmd2 = argv[2];

	ssize_t fd = open("log", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);
	if(fd == -1)
		err(1, "error while opening log file");
	close(fd);

	const pid_t child_pid1 = fork();
	if(child_pid1 == -1)
		err(2, "could not fork (1st child)");

	if(child_pid1 == 0) // we are in child for cmd1
		if(execlp(cmd1, cmd1, (char *)NULL) == -1)
			err(3, "could not execlp %s", cmd1);

	int status1;
	if(wait(&status1) == -1)
		err(4, "could not wait for 1st child");

	if(WIFEXITED(status1)){
		if(WEXITSTATUS(status1) == 0){
			if((fd = open("log", O_WRONLY | O_APPEND)) == -1)
				err(5, "error while opening log file");
			
			ssize_t cmd1_len = (ssize_t)strlen(cmd1);
			if(write(fd, cmd1, cmd1_len) != cmd1_len){
				const int olderrno = errno;
				close(fd);
				errno = olderrno;
				err(6, "could not write to log file (in 1st child)");
			}
			write(fd, "\n", 1); 
		} else warnx("%s exit status not 0", cmd1);
	} else warnx("%s did not terminate normally", cmd1);

	const pid_t child_pid2 = fork();
	if(child_pid2 == -1)
		err(7, "could not fork (2nd child)");

	if(child_pid2 == 0) // we are child for cmd2, and the child for cmd1 is finished
		if(execlp(cmd2, cmd2, (char *)NULL) == -1)
			err(8, "could not execlp %s", cmd2);

	int status2;
	if(wait(&status2)== -1)
		err(9, "could not wait for 2nd child");

	if(WIFEXITED(status2)){
		if(!WEXITSTATUS(status2)){
			if((fd = open("log", O_RDWR | O_APPEND)) == -1)
				err(10, "error while opening log file");
				
			ssize_t cmd2_len = (ssize_t)strlen(cmd2);
			if(write(fd, cmd2, cmd2_len) != cmd2_len){
				const int olderrno = errno;
				close(fd);
				errno = olderrno;
				err(11, "could not write to log file (in 2nd child)");
			}
			write(fd, "\n", 1);
		} else warnx("%s exit status not 0", cmd2);
	} else warnx("%s did not terminate normally", cmd2);
	exit(0);
}
