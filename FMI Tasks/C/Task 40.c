// github.com/andy489

#include <stdlib.h>
#include <err.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

uint8_t isdig(const char c){
	if(c>='0' && c<='9') return 1;
	return 0;
}

void usage() {
	errx(1, "Usage: <this executable> (-c n|-c n-m|-d c -f n|-d c -f n-m)");
}

int parse_digit(char symbol) {
	if (!isdig(symbol)) {
		usage();
	}

	return symbol - '0';
}

int parse_number(char** string) {
	int n = 0;
	for (; isdig((*string)[0]); (*string)++) {
		n = n * 10 + parse_digit((*string)[0]);
	}

	return n;
}

void parse_range(char* arg, int* from, int* to) {
	// arg is either 'A-B' or 'A'
	if (!isdig(arg[0]) && arg[0] != '-') {
		usage();
	}

	*from = parse_number(&arg);
	if (arg[0] == '\0') {
		*to = *from;
	} else if (arg[0] == '-') {
		arg++;
		if (arg[0] == '\0') {
			*to = 1 << 15;
		} else {
			*to = parse_number(&arg);
		}

		if (arg[0] != '\0') {
			usage();
		}
	} else {
		usage();
	}
}

void cut(int start, int end, bool use_delim, char delim) {
	char ch;
	int cnt = 0;

	while(read(0, &ch, sizeof(ch)) > 0){
		if ((!use_delim) || (ch == delim) || (cnt == 0)) {
			++cnt;
		}

		if (ch == '\n') {
			write(1, "\n", 1);
			cnt = 0;
		} else if (cnt >= start && cnt <= end) {
			if (!((cnt == start) && (ch == delim) && use_delim)) {
			    write(1, &ch, sizeof(ch));
			}
		}
	} 
	
}

int main(int argc, char **argv){
	int     start = -1, end = -1;
	char    delim = '\0';

	if ((argc == 3) && (strcmp(argv[1], "-c") == 0)) {
		parse_range(argv[2], &start, &end);
		cut(start, end, false, delim);
	} else if ((argc == 5) && (strcmp(argv[1], "-d") == 0) && (strcmp(argv[3], "-f") == 0)){
		parse_range(argv[4], &start, &end);
		delim = argv[2][0];
		cut(start, end, true, delim);
	} else {
		usage();
	}
	exit(0);
}
