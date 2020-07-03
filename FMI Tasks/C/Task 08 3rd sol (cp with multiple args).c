#include <stdlib.h>
#include <err.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

// 99 means system error and we abort
// 1 is regular error and we continue to next file to copy

char *dst(char * file, char* dir){
	size_t len1 = strlen(file);
	size_t len2 = strlen(dir);

	char *dst = malloc(len1 + 1 + len2 + 1);
	
	if(dst == NULL)
		return NULL;
	
	strcpy(dst, dir);
	strcat(dst, "/");
	strcat(dst, file);

	return dst;
}


int cpy(char *file, char* dir){
	struct stat st;	

	if(stat(file, &st) == -1)
		return 99;	
	
	if(!(st.st_mode & S_IRUSR))
		return 1;

	ssize_t fd = open(file, O_RDONLY);
	if(fd == -1)
		return 99; // 99 - system error

	char *dest = dst(file, dir);
	if(dest == NULL)
		return 99;

	ssize_t fdnew = open(dest, O_CREAT | O_TRUNC | O_WRONLY, st.st_mode);
	if (fdnew == -1){
		close(fd);
		free(dest);
		return 99;		
	}

	char buf[1<<10];
	ssize_t read_sz;
	while((read_sz = read(fd, &buf, sizeof(buf))) > 0){
		if(write(fdnew, &buf, read_sz) != read_sz){
			close(fd);
			close(fdnew);
			free(dest);
			return 99;
		}
	}
	if(read_sz == -1){ 
		close(fd);
		close(fdnew);
		free(dest);
		return 99;
	}
	close(fd);
	close(fdnew);
	free(dest);
	return 0;
}

int main(int argc, char **argv){
	if(argc < 3)
		errx(1, "Invalid number of arguments. Usage: %s <file1> [<file2> ... ] <dir>", argv[0]);

	char *dir = argv[argc - 1];

	struct stat stdir;

	if(stat(dir, &stdir) == -1)
		err(2, "error while stat %s", dir);

	if(!(stdir.st_mode & S_IFDIR) || !(stdir.st_mode & S_IWUSR))
		errx(3, "last argument is not a valid directory or is not writable");

	uint16_t i = 1;
	for(; i < argc - 1; ++i){
		int res = cpy(argv[i], dir);
		if(res == 99){
			errx(99, "system error occured while cp file %s", argv[i]);
		} else if(res == 1){
			char buf[22 + strlen(argv[i])];
			sprintf(buf, "~failed to copy file %s\n", argv[i]);
			write(2, &buf, sizeof(buf));
		} else {
			char buf[36 + strlen(argv[i]) + strlen(dir)];
			sprintf(buf, "~file %s copied to file %s successfully\n", argv[i], dir);
			write(1, &buf, sizeof(buf));
		}
	}
	exit(0);
}
