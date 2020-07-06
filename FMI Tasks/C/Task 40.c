// github.com/andy489

#include <stdlib.h>
#include <sys/types.h>
#include <err.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

void usage(const char *executable){
	errx(1, "Usage: %s [-c n | -c n-m | -d t -f n | -d t -f n-m]", executable);
}

bool isdig(const char c){
	if(c>='0' && c<='9')
		return true;
	return false;
}

ssize_t parse_digit(char symbol){
	if(!isdig(symbol)) 
		usage("this executable");
	return symbol - '0';
}

ssize_t parse_number(char **str){
	ssize_t n = 0;
	for(; isdig((*str)[0]); ++(*str))
		n = n * 10 + parse_digit((*str)[0]);
	return n;
}

void parse_range(char *arg, ssize_t *from, ssize_t *to){
	// arg is either 'n' or 'n-m'
	if(!isdig(arg[0]) && arg[0] != '-')
		usage("this executable");

	*from = parse_number(&arg);
	if(arg[0] == '\0')
		*to = *from;
	else if(arg[0] == '-'){ 
		++arg;
		if(arg[0] == '\0')
			*to = 1<<15;
		else
			*to = parse_number(&arg);

		if(arg[0] != '\0')
			usage("this executable");
	} else 
		usage("this executable");
}

void cut(ssize_t start, ssize_t end, bool use_delim, char delim){
	char ch;
	ssize_t cnt = 0, read_sz;

	while((read_sz = read(0, &ch, sizeof(ch))) == 1){
		if((!use_delim) || (ch == delim) || (cnt == 0))
			++cnt;

		if(ch == '\n'){
			write(1, "\n", 1);
			cnt = 0;
		} else if(cnt >= start && cnt <= end){
			if(!((cnt == start) && (ch == delim) && use_delim))
				write(1, &ch, 1);
		}
	}
	if(read_sz == -1)
		err(2, "could not read from stdin");
}

int main(int argc, char **argv){
	ssize_t start = -1, end = -1;
	char delim = '\0';
	if((argc == 3) && (strcmp(argv[1], "-c") == 0)){
		parse_range(argv[2], &start, &end);
		cut(start, end, false, delim);
	} else if((argc == 5) && (strcmp(argv[1], "-d") == 0)){
		parse_range(argv[4], &start, &end);
		delim = argv[2][0];
		cut(start, end, true, delim);
	} else
		usage(argv[0]);
	exit(0);
}
