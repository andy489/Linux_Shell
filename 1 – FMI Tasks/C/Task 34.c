// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

void clo(int fd1, int fd2, int fd3, int fd4){
   if(fd1 != -1) close(fd1);
   if(fd1 != -1) close(fd2);
   if(fd1 != -1) close(fd3);
   if(fd1 != -1) close(fd4);
}

int main(int argc, char** argv){
   if(argc !=5 ){
      errx(1,"usage: %s <f1.dat> <f1.idx> <f2.dat> <f2.idx>", argv[0]);
   }

   const char *f1_dat = argv[1], *f1_idx = argv[2];
   char *f2_dat = argv[3], *f2_idx = argv[4];

   struct stat f1_dat_stat;

   if(stat(f1_dat, &f1_dat_stat) == -1){
      err(2,"error while stat %s", f1_dat);
   }

   if(f1_dat_stat.st_size == 0){
      exit(0);
   }

   struct stat f1_idx_stat;

   if(stat(f1_idx, &f1_idx_stat) == -1){
      err(3,"err while stat %s", f1_idx);
   }

   if(f1_idx_stat.st_size % sizeof(uint32_t) != 0){
      errx(4,"error with length of file %s", f1_idx);
   }

   int fd1_dat = open(f1_dat, O_RDONLY);
   if(fd1_dat == -1){
      err(5,"error while opening %s", f1_dat);
   }

   int fd1_idx = open(f1_idx, O_RDONLY);
   if(fd1_idx == -1){
      int _errno=errno;
      close(fd1_dat);
      errno=_errno;
      err(6, "error while opening %s", f1_idx);
   }

   int fd2_dat = open(f2_dat, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
   if(fd2_dat == -1){
      int _errno=errno;
      clo(fd1_dat,fd1_idx, -1, -1);
      errno=_errno;
      err(7,"error while opening %s", f2_dat);
   }

   int fd2_idx = open(f2_idx, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
   if(fd2_idx == -1){
      int _errno=errno;
      clo(fd1_dat, fd1_idx, fd2_dat, -1);
      errno=_errno;
      err(8,"error while opening %s", f2_idx);
   }

   struct tripple_t{
        uint16_t pos;
        uint8_t len;
        uint8_t reserved;
    };

   struct tripple_t tr1;
   struct tripple_t tr2;

   uint8_t buf;
   int pos = 0;
   int len = 0;
   int read_sz1 = -1;
   int firstLet = 1;
  
   while((read_sz1=read(fd1_idx, &tr1, sizeof(tr1))) == sizeof(tr1)){
      firstLet = 1;
      len = 0;
      int ls = lseek(fd1_dat, tr1.pos, SEEK_SET);
      if(ls == -1){
         int _errno=errno;
         clo(fd1_dat, fd1_idx, fd2_dat, fd2_idx);
         errno=_errno;
         err(9, "error while lseek %s", f1_dat);
      }
      int read_sz2 = -1;
      while((read_sz2=read(fd1_dat, &buf, sizeof(buf)) > 0)){
         if(read_sz2 == -1){
            int _errno=errno;
            clo(fd1_dat, fd1_idx, fd2_dat, fd2_idx);
            errno=_errno;
            err(10, "error while reading %s", f1_dat);
         }
         if((buf < 'A' || buf > 'Z') && firstLet ){
            break;
         }
         tr2.pos =  pos;

         int wr_size = write(fd2_dat, &buf, sizeof(buf));
         if(wr_size == -1){
            int _errno=errno;
            clo(fd1_dat, fd1_idx, fd2_dat, fd2_idx);
            errno=_errno;
            err(11, "error while writing %s", f2_dat);
         }
         ++len;
         firstLet = 0;

         if(len == tr1.len){
            tr2.len = len;
            tr2.reserved = 0;
            pos = pos + len;
            wr_size = write(fd2_idx, &tr2, sizeof(tr2));
            if(wr_size == -1){
	       int _errno=errno;
               clo(fd1_dat, fd1_idx, fd2_dat, fd2_idx);
               errno=_errno;
               err(12, "error while writing %s", f2_idx);
            }
            break;
         }
      }
   }
   if(read_sz1==-1){
      int _errno=errno;
      clo(fd1_dat, fd1_idx, fd2_dat, fd2_idx);
      errno=_errno;
      err(13, "error while reading %s", f1_idx);
   }
   exit(0);
}
