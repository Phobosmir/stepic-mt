#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>


int main (int argc, char **argv) {
	mqd_t mqdes = mq_open("/test.mq", O_RDWR | O_CREAT | O_EXCL);
	if (mqdes == (mqd_t)-1 ) {
		perror("mq_open");
		exit(EXIT_FAILURE);
	}
	char buffer[8192];
	ssize_t bytes_received = mq_receive(mqdes, buffer, 8192, 0);
	if (bytes_received == -1 ) {
		perror("mq_receive");
		exit(EXIT_FAILURE);
	}
	if (mq_close(mqdes) == -1) {
		perror("mq_close");
		exit(EXIT_FAILURE);
	}
	if (mq_unlink("/test.mq") == -1 ) {
		perror("mq_unlink");
		exit(EXIT_FAILURE);
	}
	int fd = open("/home/box/message.txt", O_WRONLY | O_CREAT | O_EXCL);
	if (fd == -1 ) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	int bytes_written = write(fd, buffer, bytes_received);
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
	
	
		
