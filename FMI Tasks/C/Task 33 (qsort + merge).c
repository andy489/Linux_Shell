// github.com/andy489

/*
PROBLEM:
We have 256 MB RAM, which is 256 * 1024 * 1024 bytes = 268697600 bytes
on the other hand we have file with max 100000000 uint32_t numbers, which equals 400000000 bytes,
so we are 400000000 - 268697600 = 131302400 bytes short to do it with only one qsort.
We must divide the original file into two parts of at most 200000000 bytes - that will do the trick.

  	Here is the pseudo code for the algorithm:
  
	file.bin - input file that contains uint32_t elements
	sorted.bin - output file (file.bin with sorted uint32 elements)
	temp1, temp2 -- temp files that will store the two parts of the original file with sorted elements
	Having those two sorted files temp1 and temp2, we can merge them into sorted.bin and have the original file sorted.
	
	Merging arrays A and B into C:
	A,	B -> C
	n	m	
	
	i = 0
	j = 0
	k = 0
	while (i < n && j < m) {
		if A[i] <= B[j] 
			C[k++] = A[i++]
		else
			C[k++] = B[j++]
	}
	while (i < n) 
		C[k++] = A[i++]
	while (j < m) 
		C[k++] = B[j++]
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int cmp(const void *a, const void* b){
	if ( *((uint32_t*)a) > *((uint32_t*)b) ) 
		return 1;
	else if ( *((uint32_t*)a) < *((uint32_t*)b) )
		return -1; 
	return 0;
}

int main(int argc, char **argv){
	if(argc != 3)
		errx(1, "Invalid number of arguments. Usage: %s <file.bin> <sorted.bin>", argv[0]);
	
	char *bin = argv[1], *sorted = argv[2];

	struct stat st;
	if(stat(bin, &st) == -1)
		err(2, "error while stat %s", bin);

	if (st.st_size % sizeof(uint32_t) != 0)
		errx(3, "file %s is corrupted", bin);
	
	uint32_t numel = st.st_size / sizeof(uint32_t);
	
	printf("total uint32_t elements: %u\n", numel);

	uint32_t half = numel/2;
	uint32_t *p = malloc(half * sizeof(uint32_t));
	if (!p)
		err(4, "not enough memory");
	
	ssize_t fd1 = open(bin, O_RDONLY);
	if(fd1 == -1){
		free(p);
		err(5, "error while opening file %s", bin);
	}

	ssize_t t1 = open("temp1", O_CREAT | O_RDWR | O_TRUNC , 0644);
	if(t1 == -1){
		const int olderrno = errno;
		free(p);
		close(fd1);
		errno = olderrno;
		err(6, "error while opening file temp1");
	}
	
	ssize_t res = read(fd1, p, half * sizeof(uint32_t));
	if (res != half * sizeof(uint32_t)){
		const int olderrno = errno;
		free(p);
		close(fd1);
		close(t1);
		errno = olderrno;
		err(7, "error while reading file %s", bin);
	}
		
	qsort(p, half, sizeof(uint32_t), cmp);	

	res = write(t1, p, half * sizeof(uint32_t));	
	if (res != half * sizeof(uint32_t)){
		const int olderrno = errno;
		free(p);
		close(fd1);
		close(t1);
		errno = olderrno;
		err(8, "error while writing to file temp1");
	}
	
	free(p);
	
	uint32_t rhalf = numel-half;
	
	uint32_t *rp = malloc(rhalf * sizeof(uint32_t));
	
	if (!rp){
		const int olderrno = errno;
		close(fd1);
		close(t1);
		errno = olderrno;
		err(9, "not enough memory");
	}
	
	ssize_t t2 = open("temp2", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // same as 0644
	if(t2 == -1){
		const int olderrno = errno;
		close(fd1);
		close(t1);
		free(rp);
		errno = olderrno;
		err(10, "error while opening file temp2");
	}

	res = read(fd1, rp, rhalf * sizeof(uint32_t));
	if(res != rhalf * sizeof(uint32_t)){
		const int olderrno = errno;
		close(fd1);
		close(t1);
		close(t2);
		free(rp);
		errno = olderrno;
		err(11, "error while reading file %s", bin);
	}

	qsort(rp, rhalf, sizeof(uint32_t), cmp);

	res = write(t2, rp, rhalf * sizeof(uint32_t));
	if (res != rhalf*sizeof(uint32_t)){
		const int olderrno = errno;
		close(fd1);
		close(t1);
		close(t2);
		free(rp);
		errno = olderrno;
		err(12,"error while writing to file temp2");
	}

	free(rp);
	close(fd1);

	lseek(t1, 0, SEEK_SET); //TODO: check lseek
	lseek(t2, 0, SEEK_SET); //TODO: check lseek
	
	ssize_t fd2 = open(sorted, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd2 == -1){
		const int olderrno = errno;
		close(t1);
		close(t2);
		errno = olderrno;
		err(13,"error while opening file %s", sorted);
	}

	uint32_t a, b;
	size_t d1 = 0, d2 = 0, d3 = 0;

	ssize_t rd1 = -1, rd2 = -1;
	while (((rd1 = read(t1, &a, sizeof(a))) == sizeof(a)) && ((rd2 = read(t2, &b, sizeof(b))) == sizeof(b))) {
		if ( a <= b) {
			write(fd2, &a, sizeof(a)); //TODO: check write
			lseek(t2, -1 * sizeof(b), SEEK_CUR);
			//printf("write from A, turn back B\n");
		} else {
			write(fd2, &b, sizeof(b)); //TODO: check write
			lseek(t1, -1 * sizeof(a), SEEK_CUR);
			//printf("write from B, turn back A\n");
		}
		d1++;
	}
	/*
		printf("out of loop, final:\n");
		printf("rd1: %lu\n", rd1);
		printf("rd2: %lu\n", rd2);
	*/
	
	/*
		Here is the tricky part: if the first reader rd1 read one uint32_t element,
		but the second one did not, it will break the cycle and we will loose one number.
		So we need to check if this is the case and recover that number.

		Notice that there is no other corner case, since there cannot be a case in which
		the second reader rd2 read one uint32_t element, but the first did not - if the
		first condition is not true, then it will not check the second.
	*/

	if (rd1 == sizeof(a)){ 
	/* we are out of the cycle and we check if the first reader has captured an element
	   - here is where the real headache hides, if you do not consider it. */
		write(fd2, &a, sizeof(a));
		++d2;
	}

	if (rd1 == -1 || rd2 == -1){
		const int olderrno = errno;
		close(t1);
		close(t2);
		close(fd2);
		errno = olderrno;
		err(14, "error while reading from temp files");
	}

	printf("combined write: %ld\n", d1);

	while ((rd1 = read(t1, &a, sizeof(a))) == sizeof(a)) {
		write(fd2, &a, sizeof(a)); //TODO: check write
		d2++;
	}
	if (rd1 == -1){
      const int olderrno = errno;
      close(t1);
      close(t2);
      close(fd2);
      errno = olderrno;
      err(15, "error while reading from file temp1");
   }
	
	printf("left A write: %ld\n", d2);
	
	while ((rd2 = read(t2, &b, sizeof(b))) == sizeof(b)) {
		write(fd2, &b, sizeof(b)); //TODO: check write
		d3++;
	}
	if (rd2 == -1){
      const int olderrno = errno;
      close(t1);
      close(t2);
      close(fd2);
      errno = olderrno;
      err(16, "error while reading from files temp2");
   }
	
	printf("left B write: %ld\n", d3);

	close(t1);
	close(t2);
	close(fd2);
	exit(0);
}
