// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

ssize_t contains(char buf, char *set){
	ssize_t i = 0;
	while(set[i] != '\0'){
		if(set[i] == buf) return i;
		++i;
	}
	return -1;
}

int main(int argc , char **argv){
	if(argc != 3)
		errx(1, "Invalid number of arguments. Usage: %s [OPTION: -d -s] SET1 [SET2]", argv[0]);

	uint8_t d = 0, s = 0; // we use uint8_t as bool
	
	if(strcmp(argv[1], "-d") == 0) d = 1;
	if(strcmp(argv[1], "-s") == 0) s = 1;

	char *set1 = NULL, *set2 = NULL;

	ssize_t read_sz, write_sz;
	char buf;

	if(d){ // delete option
		set1 = argv[2];
		while((read_sz = read(0, &buf, 1)) == 1){
			if(contains(buf, set1) == -1)
				if((write_sz = write(1, &buf, 1)) != 1)
					err(2, "failed to write to STDOUT in \"-d\" option");
		}
		if(read_sz == -1) 
			err(3, "failed to read from STDIN in \"-d\" option");
		exit(0);
	}

	if(s){ // squeeze option
		set1 = argv[2];
		char last = '\0';
		while((read_sz = read(0, &buf, 1)) == 1){
			if(contains(buf, set1) == -1){
				if((write_sz = write(1, &buf, 1)) != 1)
					err(4, "failed to write to STDOUT in \"-s\" option");
			} else {
				if(buf != last)
					if((write_sz = write(1, &buf, 1)) != 1)
						err(5, "failed to write to STDOU in \"-s\" option");
			}
			last = buf;	
		}
		if(read_sz == -1)
			err(7, "failed to read from STDIN in \"-s\" option");
		exit(0);
	}

	// if we are here then we are in substotute option: ./main [SET1] [SET2]
	set1 = argv[1], set2 = argv[2];
	
	if(strlen(set1) != strlen(set2))
		errx(8, "SET1: [%s] and SET2: [%s] must have equal length", set1, set2);

	while((read_sz = read(0, &buf, 1)) == 1){
		ssize_t pos = contains(buf, set1);
		if(pos != -1)
			if((write_sz = write(1, &set2[pos], 1)) != 1)
				err(9, "failed to write to STDIN in \"[SET1]\" \"[SET2]\" option");
		if(pos == -1)
			if((write_sz = write(1, &buf, 1)) != 1)
				err(10, "failed to write to STDOUT in \"[SET1]\" \"[SET2]\" option");
	}
	if(read_sz == -1)
		err(11, "failed to read from STDIN in \"[SET1]\" \"[SET2]\" option");
	
	exit(0);
}
