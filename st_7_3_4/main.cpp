#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
void* lock_cond(void *cond_var) {
	printf("Condition variable: locking\n");
	pthread_cond_t *cond = (pthread_cond_t*)cond_var;
	if (cond == NULL){
		fprintf(stderr, "Condition variable is null\n");
		exit(EXIT_FAILURE);
	}
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_wait(cond, &mutex);
	printf("Confition variable: unlocked\n");
}

void* lock_barrier(void *barr) {
	printf("Barrier: locking\n");
	pthread_barrier_t* barrier = (pthread_barrier_t*)barr;
	if (barrier == NULL){
		fprintf(stderr, "Barrier is null\n");
		exit(EXIT_FAILURE);
	}
	pthread_barrier_wait(barrier);
	printf("Barrier is passed\n");
}


int main (int argc, char** argv) {
	const char save_path[] = "//home//box//main.pid";
//	const char save_path[] = "//home//pbs//main.pid";
	FILE* fd = fopen(save_path, "w");
	if (fd == NULL){
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	pid_t pid = getpid();
	int bytes_written = fprintf(fd, "%d", pid);
	if (bytes_written < 0) {
		perror("fprintf");
		exit(EXIT_FAILURE);
	}
	pthread_t thr1, thr2;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	pthread_barrier_t barrier;
	pthread_barrier_init(&barrier, NULL, 2);
	pthread_create(&thr1, NULL, lock_cond, (void*)&cond);
	pthread_create(&thr2, NULL, lock_barrier, (void*)&barrier);

	sleep(5);

	// Cleanup
	pthread_cond_signal(&cond);
	pthread_barrier_wait(&barrier);
	pthread_join(thr1, NULL);
	pthread_cond_destroy(&cond);
	return 0;
}
