#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int x = 0;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
pthread_mutex_t condLock = PTHREAD_MUTEX_INITIALIZER;
struct timespec timeout;

void* poll(void* param){
	printf("new thread\n");
	timeout.tv_sec = 20;
	timeout.tv_nsec = 0;
	
	while(1){
		sleep(2);
		pthread_mutex_lock(&condLock);
		while(x == 0){
			printf("new thread starting wait\n");
			pthread_cond_wait(&condition, &condLock);
			//pthread_cond_timedwait(&condition, &condLock, &timeout);
		}
		pthread_mutex_unlock(&condLock);
		
		/* process */
		printf("poll thread processing\n");
	}
	
	return NULL;
}

int main (int argc, const char * argv[]) {
	pthread_t pollThread;
	pthread_create(&pollThread, NULL, poll, NULL);
	int i;
	for(i = 0; i < 3; i++){
		sleep(1);
		printf("main thread\n");
	}
	
	sleep(2);
	pthread_mutex_lock(&condLock);
	x++;
	pthread_mutex_unlock(&condLock);
	pthread_cond_signal(&condition);
	
	sleep(10);
	x--;
	while(1);
    return 0;
}
