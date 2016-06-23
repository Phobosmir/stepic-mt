#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int main (int argc, char **argv) {
	int sv[2];
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv)) {
		perror("socketpair");
		exit(EXIT_FAILURE);
	}
	
	if(fork()){
		close(sv[0]);	
		struct sigaction act;
		act.sa_handler = SIG_DFL;
		act.sa_flags = SA_NOCLDWAIT;
		sigaction(SIGCHLD, &act, 0);
	} else {
		close(sv[1]);	
	}
	char c = getchar();
	return 0;
}
