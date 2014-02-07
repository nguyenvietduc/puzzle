// using pthread_mutex_t
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;
long sum;

void* thread_sum(void*);

int main(int argc, char* argv[]) {
	pthread_mutex_init(&mutex, NULL);
	pthread_t* threads;
	long num_threads = 10, thread_id;

	sum = 0;
	num_threads = atoi(argv[1]);

	threads = (pthread_t*)malloc(sizeof(pthread_t)*num_threads);
	for(thread_id=0; thread_id<num_threads;++thread_id)
		pthread_create(&threads[thread_id], NULL, thread_sum, (void*)thread_id);

	for(thread_id=0; thread_id<num_threads;++thread_id)
		pthread_join(threads[thread_id], NULL);

	printf("Final sum = %ld\n", sum);
	free(threads);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void* thread_sum(void* id) {
	long my_id = (long) id;
	pthread_mutex_lock(&mutex);
	sum += my_id;
	pthread_mutex_unlock(&mutex);
	return NULL;
}