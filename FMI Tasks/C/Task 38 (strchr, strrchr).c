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
      errx(1,"Usage: %s [OPTION] SET1 [SET2]", argv[0]);

   int d, s;
   const char *set1 = NULL, *set2 = NULL;

   d = (strcmp(argv[1],"-d") == 0)? 1:0;
   s = (strcmp(argv[1],"-s") == 0)? 1:0;

   if (!d && !s){
      set1 = argv[1];
      set2 = argv[2];
   } else set1 = argv[2];
   
   ssize_t rd = -1;
   char c;

   if(d){
      while( (rd = read(0, &c, 1)) == 1){
         if(strchr(set1, c) != NULL) continue;
         printf("%c", c);
      }
      if(rd == -1) err(2,"error while reading from STDIN");
   }
   else if(s){
      char prev = '\0';
      uint8_t first = 1;
      while( (rd = read(0, &c, 1)) == 1){
         if(c != prev) first = 1;
         if(strchr(set1, c)){
            if(!first) continue;
            printf("%c", c);
            first = 0;
         }
         else
            printf("%c", c);
         prev = c;
      }
      if(rd == -1) err(3,"error while reading from STDIN");
   }
   else{
      if(strlen(set1) != strlen(set2))
         errx(4,"sizes of sets are not consistent");

      while( (rd = read(0, &c, 1)) == 1){
         char *at = strrchr(set1, c);
         if(at)
            printf("%c", set2[(int)(at-set1)]);
         else
            printf("%c", c);
      }
      if(rd == -1) err(5,"error while reading from STDIN");
   }
   exit(0);
}
