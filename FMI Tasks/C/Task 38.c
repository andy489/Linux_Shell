// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
	if(argc != 3)
		errx(1, "Invalid number of arguments. Usage: %s [OPTION] SET1 [SET2]", argv[0]);

	int d, s;
	char *set1 = NULL, *set2 = NULL;

	d = (strcmp(argv[1],"-d") == 0)? 1:0;
	s = (strcmp(argv[1],"-s") == 0)? 1:0;

	if (!d && !s){
		set1 = argv[1];
		set2 = argv[2];
	} else set1 = argv[2];

	if(d){  //printf("we are in option: -d (errors start from 10)\n");
		ssize_t len = strlen(set1);
		ssize_t rd = -1;
		char c;
		ssize_t i;
		OUTTER1: while( (rd = read(0, &c, 1)) == 1){
			for(i = 0; i < len; ++i)
				if(c == set1[i]) 
					goto OUTTER1;
			ssize_t wr = write(1, &c, 1);
			if(wr == -1)
				err(10, "error while writing to STDOUT");		
		}
		if(rd == -1)
			err(11, "error while reading from STDIN");
		exit(0);
	}

	if(s){  //printf("we are in option: -s (errors start from 20)\n");
		ssize_t len = strlen(set1);
		ssize_t rd = -1, wr = -1;
		char c, old_c = '\0';
		ssize_t i, flag = 1;
		OUTTER2: while ( (rd = read(0, &c, 1)) == 1){
			if(flag) {
				wr = write(1, &c, 1);
				if(wr == -1)
					err(20,"error while writing to STDOUT");
				flag = 0;
				old_c = c;
				continue;
			}
			for(i = 0; i < len; ++i){
				if(set1[i] == c && old_c == c)
					goto OUTTER2;
			}
			wr = write(1, &c, 1);
			if(wr == -1)
				err(21, "error while writing to STDOUT");
			old_c = c;
		}
		if(rd == -1)
			err(22, "error while reading from STDIN");
		exit(0);
	}

	//printf("we are in option 3: [SET1] [SET2] (errors start from 30)\n");	

	if(strlen(set1) != strlen(set2))
		errx(30, "sizes of sets are not consistent");

	ssize_t len = strlen(set1);	
	ssize_t rd = -1, wr = -1;
	char c;
	ssize_t i;
	while( (rd = read(0, &c, 1)) == 1){
		char copy = c;
		for(i = 0; i < len; ++i){
			if(copy == set1[i])
				c = set2[i];
		}
		wr = write(1, &c, 1);
		if(wr == -1)
			err(31, "error while writing to STDOUT");
	}
	if(rd == -1)
		err(32,"error while reading from STDIN");
	exit(0);
}
