//github.com/andy489

#include <fcntl.h>
#include <err.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char** argv){
  
   if(argc !=3 ){
      errx(1,"Invalid number of arguments");
   }

   int fd1 = -1, fd2 = -1;

   fd1 = open(argv[1], O_RDONLY);

   if(fd1 == -1){
      err(2,"error while opening %s", argv[1]);
   }

   fd2 = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);

   if(fd2 == -1){
      int oldErrno = errno;
      close(fd1);
      errno = oldErrno;
      err(3,"error while opening %s", argv[2]);
   }

   char c[4096];
   ssize_t read_size;

   while((read_size=read(fd1, &c, sizeof(c))) > 0){
      if(write(fd2, &c, read_size) != read_size){
         close(fd1);
         close(fd2);
         errx(4,"error while writing");
      }
   }

   close(fd1);
   close(fd2);

   exit(0);
}
