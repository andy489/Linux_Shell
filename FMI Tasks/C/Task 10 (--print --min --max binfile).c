// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

int main(int argc, char** argv){
	if(argc != 3){
		errx(1,"invalid args cnt, usage: %s [--print | --min | --max] somefile.bin", argv[0]);
	}

	const char* arg = argv[1];

	if(!(strcmp(arg,"--print")==0||strcmp(arg,"--min")==0||strcmp(arg,"--max")==0)){
		errx(2,"invalid arg name, usage: %s [--print | --min | --max] somefile.bin", argv[0]);
	}
	
	const char* binfile = argv[2];
	
	int fd = open(binfile, O_RDONLY);
	if(fd == -1){
		err(3,"error while opening %s", binfile);
	}
	
	struct stat st;
	if(stat(binfile, &st) == -1){
		int _errno=errno;
		close(fd);
		errno=_errno;
		err(4,"error while stat %s", binfile);
	}	

	if(st.st_size == 0){
		printf("%s is empty\n", binfile);
		close(fd);
		exit(0);
	}
	
	if(st.st_size % sizeof(uint16_t) != 0){
		close(fd);
		printf("%s does not contain only uint16_t numbers\n", binfile);
		exit(5);
	}

	uint16_t* nums = malloc(st.st_size);
	
	if(nums == NULL){
		int _errno=errno;
		close(fd);
		errno=_errno;
		err(6,"not enough memory");
	}

	uint16_t buf;
	int read_sz;
	int i=0;
	
	while((read_sz=read(fd, &buf, sizeof(uint16_t))) > 0){
		if(read_sz!=sizeof(uint16_t)){
			int _errno=errno;
			close(fd);
			free(nums);
			errno=_errno;
			err(7,"error while reading %s", binfile);
		}
		nums[i++]=buf;
	}	

	int j;
	if(strcmp(arg,"--print") == 0){
		for(j=0;j<i;++j){
			printf("%d\n",nums[j]);
		}
	}
	else{
		uint16_t min = nums[0], max = nums[0];
		for(j=0;j<i;++j){
			if(min>nums[j]){
				min=nums[j];
			}
			if(max<nums[j]){
				max=nums[j];
			}
		}
		if(strcmp(arg,"--min")==0){
			printf("%d\n", min);
		}
		else printf("%d\n", max);
	}	

	close(fd);
	free(nums);
	exit(0);
}
