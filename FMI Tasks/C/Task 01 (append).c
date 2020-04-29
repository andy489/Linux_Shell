// Append the content file1.txt to the content of file2.txt
// github.com/andy489

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv){
        if(argc < 3 ){
            write(2, "Invalid number of arguments\n", 29);
            exit(-1);
        }

        int fd1, fd2;
        char ch;

        if((fd1 = open(argv[1], O_RDONLY)) == -1){
            write(2, "File failed to open in read mode\n", 33);
            exit(-1);
        }

        if((fd2 = open(argv[2], O_CREAT | O_WRONLY | O_APPEND)) == -1 ){
            write(2, "File failed to open in write mode\n", 34);
            exit(-1);
        }

        while(read(fd1, &ch, 1)){
            write(fd2, &ch, 1);
        }

        close(fd1);
        close(fd2);
        exit(0);
}
