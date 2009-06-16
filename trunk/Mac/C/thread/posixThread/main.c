#include <stdio.h>

#include <assert.h>
#include <pthread.h>
#include <unistd.h>

// The thread entry point routine.
void* PosixThreadMainRoutine(void* data)
{
	printf("new posixThreadID: %u \n", (unsigned int)pthread_self());
	
    return NULL;
}

void LaunchThread()
{
    // Create the thread using POSIX routines.
    pthread_attr_t  attr;
    pthread_t       posixThreadID;
	
    assert(!pthread_attr_init(&attr));
    assert(!pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));
	
    int     threadError = pthread_create(&posixThreadID, &attr, &PosixThreadMainRoutine, NULL);
	
	printf("main posixThreadID: %u \n", (unsigned int)pthread_self());
    assert(!pthread_attr_destroy(&attr));
    if (threadError != 0)
    {
		printf("threadError");
    }
	sleep(1);
}

int main(int argc, int *argv[]){
	LaunchThread();
}