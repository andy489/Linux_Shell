// Swap contents of two files
// github.com/andy489

#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    if(argc != 3) {
        fputs("Wrong number of arguments\n", stderr);
        return 1;
    }
    if(renameat2(AT_FDCWD, argv[1], AT_FDCWD, argv[2], RENAME_EXCHANGE)) {
        perror("renameat2");
        return 1;
    }
    return 0;
}
