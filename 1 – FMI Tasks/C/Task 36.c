// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int numerate = 0;
int cnt = 1;

void READ(int fd, const char * from){
    char c;
    ssize_t read_sz;

    int newline = 1;

    while((read_sz = read(fd, &c, 1)) > 0){
        if(numerate) {
            if(newline) {
                setbuf(stdout, NULL);
                fprintf(stdout, "%02d ", cnt);
                write(1, &c, sizeof(c));
                ++cnt;
                newline = 0;
            }
            else {
                write(1, &c, sizeof(c));
            }
            if(c == '\n') newline = 1;
        }
        else write(1, &c, 1);
    }
    if(read_sz == -1){
        int _errno=errno;
        close(fd);
        errno=_errno;
        err(4,"error while reading %s", from);
    }

    /*
        read ----> write
            1)
            printf(cnt)
            setbuf(3)
            write(c)

            2)
            char foo[10];
            snprintf(foo...
            write(FD, foo, n)
    */
}

int main(int argc, char ** argv){
	if(argc == 1){
		READ(0, "STDIN");
		exit(0);
	}

	int i = 1;

	if(strcmp(argv[i],"-n") == 0){
      numerate = 1;
		++i;
   }
	
	for(;i < argc; ++i){
		if(strcmp("-", argv[i]) == 0){
			READ(0, "STDIN");
			continue;
		}
		const char * filepath = argv[i];
		
		struct stat st;
		if(stat(filepath, &st) == -1){
			err(1, "error while stat %s", filepath);
		}
    
    		if(!S_ISREG(st.st_mode)){
      			errx(2, "%s is nor regular file", filepath);
    		}
    
    		int fd = open(filepath, O_RDONLY);
    		if(fd == -1){
	     		err(3,"error opening %s", filepath);
     		}  
   		// exist, regular, readable
		READ(fd, "filepath");
	}
	exit(0);
}
