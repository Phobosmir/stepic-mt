#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>


int main (int argc, char **argv) {
	key_t key = ftok("/tmp/sem.temp", 1);
	if (key == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}
	int semid = semget(key, 16, IPC_CREAT | IPC_EXCL | 0777);
	if (semid == -1) {
		perror("semget");
		exit(EXIT_FAILURE);
	}
	union semun {
		int val;
		struct semid_ds *buf;
		unsigned short *array;
		struct seminfo *__buf;
	} arg;
	
	int i;
	for (i = 0; i < 16; i++) {
		arg.val = i;
		if (semctl(semid, i, SETVAL, arg) == -1) {
			perror("semctl");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
