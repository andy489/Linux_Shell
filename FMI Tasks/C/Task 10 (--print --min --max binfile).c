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
	
	struct stat st;
	if(stat(binfile, &st) == -1){
		err(3,"error while stat %s", binfile);
	}	

	if(st.st_size == 0){
		printf("%s is empty\n", binfile);
		exit(0);
	}
	
	if(st.st_size % sizeof(uint16_t) != 0){
		printf("%s does not contain only uint16_t numbers\n", binfile);
		exit(4);
	}

	if(access(binfile, O_RDONLY) == -1){
		err(5,"error while opening %s", binfile);
	}

	uint16_t* nums = malloc(st.st_size);
	
	if(nums == NULL){
		err(6,"not enough memory");
	}

	uint16_t buf;
	int read_sz;
	int i=0;

	int fd = open(binfile, O_RDONLY);
	if(fd == -1){
		int _errno=errno;
		free(nums);
		errno=_errno;
		err(7,"error while opening %s", binfile); //double check
	}
	
	while((read_sz=read(fd, &buf, sizeof(uint16_t))) > 0){
		if(read_sz!=sizeof(uint16_t)){
			int _errno=errno;
			free(nums);
			errno=_errno;
			err(7,"error while reading %s", binfile);
		}
		nums[i++]=buf;
	}	

	int j;
	if(strcmp(arg,"--print") == 0){
		for(j=0;j<i;++j){
			printf("%x\n",nums[j]);
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
			printf("%x\n", min);
		}
		else printf("%x\n", max);
	}	
	free(nums);
	exit(0);
}
