#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
	unsigned sem_val = 66;
	sem_t *semid = sem_open("test.sem", O_CREAT | O_EXCL, 0777, sem_val);
	if (semid == SEM_FAILED) {
		perror("sem_open");
		exit(EXIT_FAILURE);
	}
	return 0;
}
