#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex;
pthread_spinlock_t spin_mutex;
pthread_rwlock_t rw_mutex;

void* pthread_dummy_job(void *arg) {
	int job_id = *((int*)arg);
	printf("job arg: %d\n", job_id);
	int status;
	switch (job_id) {
	case 0:
		status = pthread_mutex_lock(&mutex);
		break;
	case 1:
		status = pthread_spin_lock(&spin_mutex);
		break;
	case 2:
		status = pthread_rwlock_rdlock(&rw_mutex);
		break;
	case 3:
		status = pthread_rwlock_wrlock(&rw_mutex);
		break;
	default:
		status = 1;
	}
	if (status != 0) {
		perror("mutex lock");
		exit(EXIT_FAILURE);
	}
	return (void*)42;
}
void init_and_lock() {
	if (pthread_mutex_init(&mutex, 0) ||
		pthread_spin_init(&spin_mutex, PTHREAD_PROCESS_SHARED) ||
			pthread_rwlock_init(&rw_mutex, 0)){

		perror("init mutex");
		exit(EXIT_FAILURE);
	}
	if (pthread_mutex_lock(&mutex) || pthread_spin_lock(&spin_mutex) ||
		pthread_rwlock_rdlock(&rw_mutex) || 
		pthread_rwlock_wrlock(&rw_mutex)) {

		
		perror("lock in main thread");
		exit(EXIT_FAILURE);
	} 

	printf("init and lock successful\n");
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
	init_and_lock(); 
	int i;
	int pthread_arg[4];
	for (i = 0; i < 4; i++) {
		pthread_t pthread_id;
		pthread_arg[i]=i;
		if (pthread_create(&pthread_id, 0, &pthread_dummy_job, &pthread_arg[i]) != 0 ) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}
	
	while(1){}
	return 0;
}

