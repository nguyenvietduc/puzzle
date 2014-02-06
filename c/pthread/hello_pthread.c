// hello_pthread.c
// The compulsory 'Hello, world' with Pthreads
//
// compile: gcc -g -Wall -o hello_pthread hello_pthread.c -lpthread
// run: ./hello_pthread <num_thread>
// 
// try to run a couple of times to see the (indeterministic) interleaving of threads.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* hello_world(void*);

int num_threads; // Global variable, shared (accessible) among threads

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("ERROR! Usage: ./hello_pthread <number_of_threads>\n");
		return -1;
	}

	long threadID;
	pthread_t* threads;

	// Get number of threads from command line
	num_threads = strtol(argv[1], NULL, 10);

	// Allocate pthread_t objects before the corresponding thread objects
	// can be created and execute.
	threads = malloc(num_threads*sizeof(pthread_t));

	// Threads creation
	// Normally threads are identified by ID or some tags
	// Here we use ID from 0 to (num_thread-1)
	for (threadID = 0; threadID < num_threads; threadID++)
		pthread_create(&threads[threadID], NULL, hello_world, (void*) threadID);

	/*
		NOTE: Check Pthread programming guide for detail syntax.
		Basically, you would pass:
			- pointer to a pthread_t object
			- thread attribute, if any
			- pointer to a function that thread will be executing
			- pointer to argument (or list of arguments) the should be
			passed to the executing function
	*/

	printf("Hello, world! This is main thread.\n");

	// Pthreads run automatically. Programmers have no control over this, even
	// though, some distributions of Linux allows so. However, that is not portable.

	// wait for all of them to finish executing
	for (threadID = 0; threadID < num_threads-1; threadID++)
		pthread_join(threads[threadID], NULL);

	// free memory
	free(threads);
	return 0;
}

/*
	prototype for function executed by pthread
		void* thread_function(void* args_p);
	Notice that the type void* of args_p. This allows it to be cast to any pointer type in C,
	which means it can point to a list of more than one arguments needed for thread_function
*/
void* hello_world(void* threadID) {
	long my_thread_id = (long) threadID;
	printf("Hello, world! This is thread %ld among %d threads.\n", my_thread_id, num_threads);
	return NULL;
}