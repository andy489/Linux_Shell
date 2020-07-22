#include <err.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

typedef struct{
	uint32_t magic;
	uint8_t hVersion, dVersion;
	uint16_t count;
	uint8_t trash[8];
} Header;

static int checkHeader(const Header* h){
	return h->magic == 0xEFBEADDE && h->hVersion == 1 && h->dVersion <= 1;
}

typedef struct{
	uint16_t offset;
	uint8_t original, new;
} Ver1_t;

typedef struct{
	uint32_t offset;
	uint16_t original, new;
} Ver2_t;

static off_t getFileSize(const char* file){
	struct stat st;
	if(stat(file, &st) == -1)
		return -1;
	return st.st_size;
}

static off_t copyContent(int fdSrc, int fdDest){
	char buf[1<<9];
	off_t bytesCopied = 0;
	ssize_t rd;
	while((rd = read(fdSrc, &buf, sizeof(buf))) > 0){
		if(write(fdDest, &buf, rd) != rd)
			return -1;
		bytesCopied += rd;
	}
	return bytesCopied;
}

static void clo(int errcode, int errnum, int fdP, int fdIn, int fdOut, const char *msg){
	if(fdP != -1) close(fdP);
	if(fdIn != -1) close(fdIn);
	if(fdOut != -1) close(fdOut);
	if(errnum == 0){
		errx(errcode, "%s", msg);
	} else {
		errno = errnum;
		err(errcode, "%s", msg);
	}
}

int main(int argc, char** argv){
	if(argc != 4)
		errx(1, "Invalid number of arguments. Usage: %s <patchfile> <inputfile> <outputfile>", argv[0]);
	const char *patch = argv[1], *input = argv[2], *output = argv[3];
	off_t patchSz = getFileSize(patch);
	if(patchSz == -1)
		err(2, "failed to stat %s", patch);
	Header h;
	int fdP = open(patch, O_RDONLY);
	if(fdP == -1)
		err(3, "%s could not be opened", patch);
	if(read(fdP, &h, sizeof(h)) != sizeof(h))
		clo(4, errno, fdP, -1, -1, "could not read header from patch file");
	size_t sz = sizeof(Ver2_t);
	if(h.dVersion == 0)
		sz = sizeof(Ver1_t);
	if(!checkHeader(&h) || patchSz-sizeof(Header) != h.count*sz)
		clo(5, errno, fdP, -1, -1, "incorrect format of patchfile");
	off_t inpSz = getFileSize(argv[2]);
	if(inpSz == -1)
		clo(6, errno, fdP, -1, -1, "stat failed");
	if(h.dVersion == 1 && inpSz % sizeof(uint16_t))
		clo(7, errno, fdP, -1, -1, "incorrect format of inputfile");
	int fdIn = open(input, O_RDONLY);
	if(fdIn == -1)
		clo(8, errno, fdP, -1, -1, "inputfile could not be opened");
	int fdOut = open(output, O_RDWR | O_CREAT, 0644);
	if(fdOut == -1)
		clo(9, errno, fdP, fdIn, -1, "could not create outputfile");
	// duplicate the original file
	if(copyContent(fdIn, fdOut) != inpSz)
		clo(10, errno, fdP, fdIn, fdOut, "error read/write");
	while(h.count--){
		char buf[sizeof(Ver2_t)];
		if(read(fdP, &buf, sz) != (ssize_t)sz)
			clo(11, errno, fdP, fdIn, fdOut, "error read");
		uint16_t offset, originalVal, newVal, valueSz;
		if(sz == sizeof(Ver1_t)){
			Ver1_t record;
			memcpy(&record, buf, sz);
			offset = record.offset;
			originalVal = record.original;
			newVal = record.new;
			valueSz = sizeof(uint8_t);
		} else {
			Ver2_t record;
			memcpy(&record, buf, sz);
			offset = record.offset;
			originalVal = record.original;
			newVal = record.new;
			valueSz = sizeof(uint16_t);
		}
		if(offset >= inpSz/valueSz)
			clo(12, errno, fdP, fdIn, fdOut, "bad offset");
		if(lseek(fdOut, valueSz*offset, SEEK_SET) == -1)
			clo(13, errno, fdP, fdIn, fdOut, "lseek failed");
		uint16_t oldVal = 0; // initialization is to clear the second byte if only the first is used
		if(read(fdOut, &oldVal, valueSz) != valueSz)
			clo(14, errno, fdP, fdIn, fdOut, "error read");
		if(oldVal != originalVal) // assuming little endian
			clo(15, errno, fdP, fdIn, fdOut, "original value not found");
		if(lseek(fdOut, valueSz*offset, SEEK_SET) == -1)
			clo(16, errno, fdP, fdIn, fdOut, "lseek failed");
		if(write(fdOut, &newVal, valueSz) != valueSz)
			clo(17, errno, fdP, fdIn, fdOut, "error write");
		}
	close(fdOut);
	close(fdIn);
	close(fdP);
	exit(0);
}
