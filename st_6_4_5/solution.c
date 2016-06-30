#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	key_t key = ftok("/tmp/mem.temp", 1);
	if (key == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}
	int mem_size = 1024*1024;
	int shmid = shmget(key, mem_size, S_IRUSR | S_IWUSR | IPC_CREAT | IPC_EXCL);
	if (shmid == -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}
	char *mem = shmat(shmid, 0, 0);
	if (mem == (void *) -1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}
	int i;
	for (i = 0; i < mem_size; i++) {
		mem[i] = 42;
	}
	return 0;
}
