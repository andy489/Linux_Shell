// github.com/andy489

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

int main (int argc, char* argv[]){
	int fd1,
		lines = 0, 
		words = 0, 
		chars = 0;
	char c;
	
	if (argc != 2) {
		fprintf(stderr, "Invalid number of arguments\n");
		exit(1);
	}

	if ( (fd1 = open(argv[1], O_RDONLY)) == -1 ) {
		fprintf(stderr, "Operation open failed\n");
		exit(1);
	}

	int inWord = 0;
	while ( read(fd1, &c, 1) > 0 ) {
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

	printf("File %s has:\n%d number of lines.\n%d number of words.\n%d number of chars.\n", argv[1], lines, words, chars);
	close(fd1);
	exit(0);
}
