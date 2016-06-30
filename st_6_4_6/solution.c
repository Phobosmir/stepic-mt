#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
	int shmfd = shm_open("/test.shm", O_RDWR | O_CREAT | O_EXCL, 0777);
	if (shmfd == -1)  {
		perror("shm_open");
		exit(EXIT_FAILURE);
	}
	int mem_len = 1024*1024;
	if (ftruncate(shmfd, mem_len) == -1) {
		perror("ftruncate");
		exit(EXIT_FAILURE);
	}
	
	char *mem = mmap(0, mem_len, PROT_WRITE, MAP_SHARED, shmfd, 0);
	if (mem == MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	int i;
	for (i = 0; i < mem_len; i++) {
		mem[i] = 13;
	}
	if (munmap(mem, mem_len) == -1) {
		perror("munmap");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}
	
