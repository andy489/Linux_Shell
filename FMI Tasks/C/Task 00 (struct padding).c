// github.com/andy489

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <err.h>

int main(int argc, char** argv){
	printf("argv[0]: %s\n", argv[0]);
	if(argc != 2){
		errx(1, "exactly one argument needed");
	}	
	printf("argv[1]: %s\n", argv[1]);	
	
	struct foo {
		uint16_t a;
		uint8_t b;
		uint16_t c;
		uint32_t d;
	}A;
	
	//struct foo A;
	
	A.a = 3; A.b = 4; A.c = 5; A.d = 0x12345678;

	printf("\nstruct A:\na: %d, b: %d, c: %d, d: %d\n", A.a, A.b, A.c, A.d);
	printf("a size: %ld, b size: %ld, c size: %ld, d size: %ld, all size: %ld\n",
			sizeof(A.a), sizeof(A.b), sizeof(A.c), sizeof(A.d), sizeof(A));
	printf("1+2=3 byte unnamed padding [2 1 2 4 -> 4(2+1) 4(2) 4(4)]\n");
	
	struct bar {
		uint16_t a;
		uint8_t b;
		uint16_t c;
		uint32_t d;
	}__attribute__((packed))B;

	B.a = 3; B.b = 4; B.c = 5, B.d = 0x12345678;

 	printf("\nstruct B:\na: %d, b: %d, c: %d, d: %d\n", B.a, B.b, B.c, B.d);
    printf("a size: %ld, b size: %ld, c size: %ld, d size: %ld, all size: %ld\n",
            sizeof(B.a), sizeof(B.b), sizeof(B.c), sizeof(B.d), sizeof(B));
    printf("No padding\n\n");

	int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fd <=0 ){
		err(2,"error open");
	}
	//printf("file descriptor: %d\n", fd);
	
	int wa = write(fd, &A, sizeof(A));
	if(wa != sizeof(A) ){
		err(3,"err1 write");
	}
	
	int wb = write(fd, &B, sizeof(B));
	if(wb != sizeof(B) ){
		err(4,"err2 write");
	
	}	

	close(fd);
	exit(0);
}
