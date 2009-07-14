#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

int n = 0;
void timefunc(int sig) /* 定时事件代码 */
{
	//fprintf(stderr, "ITIMER_PROF[%d]\n", n++);
	printf("ITIMER_PROF[%d]\n", n++);
	signal(SIGPROF, timefunc); /* 捕获定时信号 */
}

void* testTimer(void *param){
	struct itimerval value;
	value.it_value.tv_sec=0; /* 定时1.5秒 */
	value.it_value.tv_usec=500;
	value.it_interval.tv_sec=0; /* 定时1.5秒 */
	value.it_interval.tv_usec=50;
	signal(SIGPROF, timefunc); /* 捕获定时信号 */
	setitimer(ITIMER_PROF, &value, NULL); /* 定时开始 */
	while(1);
	return NULL;
}

int main(int argc, const char* argv[])
{
	//	testTimer(NULL);
	//	
	//	pthread_t thread;
	//	pthread_create(&thread, NULL, &testTimer, NULL);
	
	struct itimerval value;
	value.it_value.tv_sec=0; /* 定时1.5秒 */
	value.it_value.tv_usec=500;
	value.it_interval.tv_sec=0; /* 定时1.5秒 */
	value.it_interval.tv_usec=50;
	signal(SIGPROF, timefunc); /* 捕获定时信号 */
	setitimer(ITIMER_PROF, &value, NULL); /* 定时开始 */
	// while(1);
	for(int i = 0; i < 1000; i++){;}
	
	//	sleep(5);
	printf("-----\n");
	return 0;
}

