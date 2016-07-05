#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* pthread_dummy_job(void *arg) {
	return (void*)42;
}

int main (int argc, char **argv) {
	const char pid_save_path[] = "/home/box/main.pid";
	FILE *file = fopen(pid_save_path, "w");
	if (file == 0) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	pid_t pid = getpid();
	if (fprintf(file, "%d", pid) < 0) {
		perror("fprintf");
		exit(EXIT_FAILURE);
	}
	if (fclose(file) != 0 ) {
		perror("fclose");
		exit(EXIT_FAILURE);
	}
	pthread_t pthread_id;
	if (pthread_create(&pthread_id, 0, &pthread_dummy_job, 0) != 0 ) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	while(1){}
	return 0;
}

