// github.com/andy489

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

int cmp(const void *a, const void* b){
	if ( *((uint32_t*)a) > *((uint32_t*)b) ) {
		return 1;
	} else if ( *((uint32_t*)a) < *((uint32_t*)b) ) {
		return -1;
	} 
	return 0;
}

int main(int argc, char **argv){
	if(argc != 3){
		err(1,"Usage: %s <file.bin> <sorted.bin>", argv[0]);
	}
	
	char *bin = argv[1], *sorted = argv[2];

	struct stat st;
	if(stat(bin, &st) == -1){
		err(2,"error while stat %s", bin);
	}

	if (st.st_size % sizeof(uint32_t) != 0){
		errx(3,"file %s is corrupted", bin);
	}
	
	uint32_t numel = st.st_size / sizeof(uint32_t);
	
	printf("number of elements: %d\n",numel);

	uint32_t half = numel/2;
	uint32_t *p = malloc(half * sizeof(uint32_t));
	if (!p){
		err(4,"not enough memory");
	}
	
	int fd1 = open(bin, O_RDONLY);
	if(fd1 == -1){
		free(p);
		err(5,"error while opening file %s", bin);
	}

	int t1 = open("temp1", O_CREAT | O_RDWR | O_TRUNC , S_IRUSR | S_IWUSR);
	if(t1 == -1){
		const int _errno = errno;
		free(p);
		close(fd1);
		errno=_errno;
		err(6,"error while opening file temp1");
	}
	
	printf("size of 1 element: %ld\n",sizeof(p));
	
	size_t res = read(fd1, p, half * sizeof(uint32_t));
	if (res != half * sizeof(uint32_t)){
		const int _errno = errno;
		free(p);
		close(fd1);
		close(t1);
		errno = _errno;
		err(7,"error while reading file %s", bin);
	}
		
	qsort(p, half, sizeof(uint32_t), cmp);	

	res = write(t1, p, half * sizeof(uint32_t));	
	if(res != half * sizeof(uint32_t)){
		const int _errno = errno;
		free(p);
		close(fd1);
		close(t1);
		errno = _errno;
		err(8,"error while writing to file temp1");
	}
	
	free(p);
	
	uint32_t rhalf = numel-half;
	
	uint32_t *rp = malloc(rhalf * sizeof(uint32_t));
	
	if(!rp){
		const int _errno = errno;
		close(fd1);
		close(t1);
		errno = _errno;
		err(9,"not enough memory");
	}
	
	int t2 = open("temp2", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(t2 == -1){
		const int _errno = errno;
		close(fd1);
		close(t1);
		free(rp);
		errno=_errno;
		err(10,"error while opening file temp2");
	}

	res = read(fd1, rp, rhalf * sizeof(uint32_t));
	if(res != rhalf * sizeof(uint32_t)){
		const int _errno = errno;
		close(fd1);
		close(t1);
		close(t2);
		free(rp);
		errno = _errno;
		err(11,"error while reading file %s", bin);
	}

	qsort(rp, rhalf, sizeof(uint32_t), cmp);

	res = write(t2, rp, rhalf * sizeof(uint32_t));
	if( res != rhalf*sizeof(uint32_t)){
		const int _errno = errno;
		close(fd1);
		close(t1);
		close(t2);
		free(rp);
		errno = _errno;
		err(12,"error while writing to file temp2");
	}

	free(rp);
	close(fd1);

	lseek(t1, 0, SEEK_SET);
	lseek(t2, 0, SEEK_SET);
	
	int fd2 = open(sorted, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fd2 == -1){
		const int _errno = errno;
		close(t1);
		close(t2);
		errno = _errno;
		err(13,"error while opening file %s", sorted);
	}

	uint32_t a, b;
	size_t d1 = 0, d2 = 0, d3 = 0;

	int rd1 = -1, rd2 = -1;
	while ((rd1 = read(t1, &a, sizeof(a)) == sizeof(a)) && (rd2 = read(t2, &b, sizeof(b)) == sizeof(b))) {
		if ( a <= b) {
			write(fd2, &a, sizeof(a)); //TODO check write
			lseek(t2, -1 * sizeof(b), SEEK_CUR);
		} else {
			write(fd2, &b, sizeof(b)); //TODO check write
			lseek(t1, -1 * sizeof(a), SEEK_CUR);
		}
		d1++;
	}
	if(rd1 == -1 || rd2 == -1){
		const int _errno = errno;
		close(t1);
		close(t2);
		close(fd2);
		errno = _errno;
		err(14, "error while reading from temp files");
	}

	printf("combined write: %ld\n", d1);

	while ((rd1 = read(t1, &a, sizeof(a))) == sizeof(a)) {
		write(fd2, &a, sizeof(a)); //TODO check write
		d2++;
	}
	if(rd1 == -1){
      const int _errno = errno;
      close(t1);
      close(t2);
      close(fd2);
      errno = _errno;
      err(15, "error while reading from file temp1");
   }
	
	printf("left A write: %ld\n", d2);
	
	while ((rd2 = read(t2, &b, sizeof(b))) == sizeof(b)) {
		write(fd2, &b, sizeof(b)); //TODO check write
		d3++;
	}
	 if(rd2 == -1){
      const int _errno = errno;
      close(t1);
      close(t2);
      close(fd2);
      errno = _errno;
      err(16, "error while reading from files temp2");
   }
	
	printf("left B write: %ld\n", d3);

	close(t1);
	close(t2);
	close(fd2);
	exit(0);
}

/*

  algorithm: pseudo code
  
	f1 - input file // uint32_t
	f2 - output file (sorted)
	t1, t2 -- temp files

	a,	b -> c
	n	m	
	
	i = 0
	j = 0
	k = 0

	while (i < n && j < m) {
		if a[i] <= b[j] {
			c[k++] = a[i++]
		} else {
			c[k++] = b[j++]
		}
	}

	while (i < n) {
		c[k++] = a[i++]
	}
	while (j < m) {
		c[k++] = b[j++]
	}
*/
