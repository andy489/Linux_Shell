// github.com/andy489

#include <unistd.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>

// f1 - pairs -> (uint32_t, uint32_t)
// f2 - data  -> uint32_t
// f3 - output

void clo(int fd1, int fd2, int fd3){
	if(fd1 != -1) close(fd1);
	if(fd2 != -1) close(fd2);
	if(fd3 != -1) close(fd3);
}

int main(){
	int fd1 = -1, fd2 = -1, fd3 = -1;
	char *f1 = "f1", *f2 = "f2", *f3 = "f3";

	struct stat st1;
   struct stat st2;

	struct pair_t {
      uint32_t first; // start pos
      uint32_t second; // length of content
   } pair;

   if(stat(f1, &st1) == -1){
      err(1,"error while stat %s", f1);
   }
  
   if(stat(f2, &st2) == -1){
      err(2,"error while stat %s", f2);
   }

	if(st1.st_size % sizeof(pair) != 0){
      errx(3,"file %s is corrupted", f1);
   }

   if(st2.st_size % sizeof(uint32_t) != 0){
      errx(4,"file %s is corrupted", f2);
   }

	if((fd1 = open(f1, O_RDONLY)) == -1){
		err(5,"error file %s not readable", f1);
	}

	if((fd2 = open(f2, O_RDONLY)) == -1){
		int _errno=errno;
		clo(fd1,fd2,fd3);
		errno=_errno;
		err(6,"error file %s not readable", f2);
	}

	if((fd3 = open(f3, O_CREAT | O_TRUNC | O_WRONLY , S_IRUSR | S_IWUSR)) == -1){
		int _errno=errno;
		clo(fd1,fd2,fd3);
		errno=_errno;
		err(7,"error, file %s not writable", f3);
	}

	int rd1 = -1;
	while((rd1 = read(fd1, &pair, sizeof(pair))) > 0){
		uint32_t from = pair.first;
		uint32_t len = pair.second;
		off_t ls = lseek(fd2, (from - 1) * sizeof(uint32_t), SEEK_SET);
		if(ls == -1){
			int _errno=errno;
			clo(fd1,fd2,fd3);
			errno=_errno;
			err(8,"error while lseek file %s", f2);
		}
		uint32_t element;
		uint32_t cnt = 0;
		int rd2 = -1;
		while((rd2 = read(fd2, &element, sizeof(element))) > 0){
			++cnt;
			int wr = write(fd3, &element, rd2);
			if(wr == -1 || wr != rd2){
				int _errno=errno;
				clo(fd1,fd2,fd3);
				errno=_errno;
				err(9,"error while writing to file %s",f3);
			}
			if(cnt == len) break;	
		}
		if(rd2 == -1){		
			int _errno=errno;
			clo(fd1,fd2,fd3);
			errno=_errno;
			err(10,"error while reading %s", f2);
		}
	}
	if(rd1 == -1){
		int _errno=errno;
		clo(fd1,fd2,fd3);
		errno=_errno;
		err(11,"error while reading from file %s", f1);
	}
	clo(fd1,fd2,fd3);
	exit(0);
}
