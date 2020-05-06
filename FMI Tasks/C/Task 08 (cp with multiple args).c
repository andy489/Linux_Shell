// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char** argv){

	if(argc < 3){
		errx(1,"invalid number of args");
	}
	
	char *dirpath = argv[argc-1]; 
	
	int fd_dir = open(dirpath, O_DIRECTORY);		
	if(fd_dir == -1){
		err(2,"error %s", dirpath);
	}
	
	int wr = access(dirpath, W_OK);
	if(wr != 0){
		err(3,"dir %s - not writable", dirpath);
	}	
	
	int s1 = (int)strlen(dirpath);
	if(dirpath[s1-1] != '/'){
		strcat(dirpath,"/");
		s1+=1;
	}
	//printf("name: %s, len: %lu\n",dirpath, strlen(dirpath));
	
	int i=1;
	for(; i < argc - 1; ++i){
		int fd = open(argv[i], O_RDONLY);
		if(fd == -1){
			printf("error. file %s not readable\n", argv[i]);
			continue;
		}
		
		int s2 = (int)strlen(argv[i]);
		char *target = malloc(s1+s2+1);
		if(target == NULL){
			int _errno=errno;
			close(fd);
			errno=_errno;
			err(4,"not enough memory");
		}
		int j=0;
		for(; j < s1; ++j){
			target[j] = argv[argc-1][j];
		}
		for(;j < s1 + s2; ++j){
			target[j] = argv[i][j-s1];
		}
		target[j] = '\0';

		//printf("target: %s\n", target);
		
		int towr = open(target, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
		if(towr == -1){
			close(fd);
			free(target);
			printf("error file %s \n", target);
		}

		char c[4096];
		ssize_t read_size;

		while((read_size = read(fd, &c, sizeof(c))) > 0){
			if(write(towr, &c, read_size) != read_size){
				printf("error while writing to file: %s\n", target);
			}
		}
		free(target);
		close(fd);
		close(towr);
	}
	exit(0);
}
