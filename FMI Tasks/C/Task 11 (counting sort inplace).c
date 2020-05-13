// github.com/andy489
// counting sort binary file - inplace

#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv){
   if (argc != 2)
      errx(1, "Usage: %s <file.bin>", argv[0]);

   char * bin = argv[1];
  
   if (access(bin, F_OK) == -1)
      err(2, "file %s doesnot exist", bin);

   if (access(bin, R_OK | W_OK) == -1)
      err(3, "file %s does not have the needed permissions for inplace sorting", bin);

   ssize_t fd = open(bin, O_RDWR);
   if (fd == -1)
      err(4, "error while opening file %s", bin);

   uint8_t counting[256] = { 0 };

   uint8_t b;
   ssize_t read_sz;
  
   while ((read_sz = read(fd, &b, sizeof(b))) == sizeof(b))
      ++counting[b];
   if (read_sz == -1){
      int _errno = errno;
      close(fd);
      errno = _errno;
      err(5, "error while reading file %s", bin);
   }

   ssize_t ls = lseek(fd, 0, SEEK_SET);
   if(ls == -1){
      int _errno = errno;
      close(fd);
      errno = _errno;
      err(6,"error while lseek file %s", bin);
   }
  
   uint16_t i;
   for (i = 0; i < 256; ++i){
      b = i;
      while (counting[i]){
         if( write(fd, &b, sizeof(b)) != sizeof(b) ){
            int _errno = errno;
            close(fd);
            errno = _errno;
            err(7, "error while writing to file %s", bin);
         }
         --counting[i];
      }
   }
   close(fd);
   exit(0);
}
