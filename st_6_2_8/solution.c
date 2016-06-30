#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv) {
	key_t key = ftok("/tmp/msg.temp", 1);
	struct message {
		long mtype;
		char mtext[80];
	} msg;
	
	int msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0777);
	if (msgid == -1 ) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	int bytes_rcvd = msgrcv(msgid, (void*)&msg, 80, 0, 0);
	if (bytes_rcvd == -1 ) {
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}	
	
	if (msgctl(msgid, IPC_RMID, 0) == -1) {
		perror("msgctl");
		exit(EXIT_FAILURE);
	}

	int fd = open("/home/box/message.txt", O_WRONLY | O_CREAT);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}	
	int bytes_written = write(fd, msg.mtext, bytes_rcvd);
	if (bytes_written == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}
	if (close(fd) == -1 ) {
		perror("close");
		exit(EXIT_FAILURE);
	}
	return 0;
}
