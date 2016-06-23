#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
	char *in_fifo, *out_fifo;
	if (argc == 3) {
		in_fifo = argv[1];
		out_fifo = argv[2];
	} else {
		in_fifo = "/home/box/in.fifo";
		out_fifo = "/home/box/out.fifo";
	}
	if (mkfifo(in_fifo, 666)) {
		perror("in.fifo");
		exit(EXIT_FAILURE);
	}
	if (mkfifo(out_fifo, 666)) {
		perror("in.fifo");
		exit(EXIT_FAILURE);
	}
	int from = open(in_fifo, O_RDONLY|O_NONBLOCK);
	int to = open(out_fifo, O_WRONLY);
	if ((from  == -1)||(to == -1)) {
		perror("open fifo");
		exit(EXIT_FAILURE);
	}
	fd_set set;
	FD_ZERO(&set);
	FD_SET(from, &set);
	char buffer[512];
	int rbytes;
	while((select(from+1, &set, 0,0,0)) != -1){
		if (FD_ISSET(from, &set)){
			rbytes = read(from, buffer, 512);
			if (rbytes == -1) {
				perror("read from fifo");
				exit(EXIT_FAILURE);
			}
			if (write(to, buffer, rbytes) == -1) {
				perror("write to fifo");
				exit(EXIT_FAILURE);
			}
		}
	}
	return 0;
}
