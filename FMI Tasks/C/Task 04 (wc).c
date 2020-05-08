// github.com/andy489

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>

int main (int argc, char* argv[]){
	int fd, lines = 0, words = 0, chars = 0;
	char c;
	
	if (argc != 2) {
		errx(1, "Invalid number of arguments");
	}

	const char * file = argv[1];

	if ( (fd = open(file, O_RDONLY)) == -1 ) {
		err(2, "error opening file %s", file);
	}

	int inWord = 0, rd;
	while (rd=read(fd, &c, 1)) {
		if(rd==-1 || rd!=1){
			int _errno=errno;
			close(fd);
			errno=_errno;
			err(3,"error while reading %s", file);
		}
		if (c=='\n') {
			++lines;
			inWord = 0;
		}

		if (c!=' ' && c!='\n' && c!='\t' && c!='\0') {
			if(!inWord){
				++words;
				inWord = 1;
			}
		} 
		else inWord = 0;
		++chars;
	}

	printf("File %s has:\n%d lines\n%d words\n%d chars\n", file, lines, words, chars);
	close(fd);
	exit(0);
}
