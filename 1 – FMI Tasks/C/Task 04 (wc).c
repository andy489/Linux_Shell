// github.com/andy489

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>

int main (int argc, char* argv[]){
	
	if (argc != 2) {
		errx(1, "Invalid number of arguments");
	}

	const char * file = argv[1];

	int fd;
	if ( (fd = open(file, O_RDONLY)) == -1 ) {
		err(2, "error opening file %s", file);
	}

	int lines = 0, words = 0, chars = 0;
	char c[2<<10];
	
	int inWord = 0, rd = -1;
	
	while((rd = read(fd, &c, sizeof(c))) > 0) {
		for(int i=0; i<rd; ++i){
			if (c[i]=='\n') {
				++lines;
				inWord = 0;
			}

			if (c[i]!=' ' && c[i]!='\n' && c[i]!='\t' && c[i]!='\0') {
				if(!inWord){
					++words;
					inWord = 1;
				}
			} 
			else inWord = 0;
			++chars;
		}
	}
	if(rd == -1){
		int _errno=errno;
		close(fd);
		errno=_errno;
		err(3,"error while reading %s", file);
	}

	printf("File %s has:\n%d lines\n%d words\n%d chars\n", file, lines, words, chars);
	close(fd);
	exit(0);
}
