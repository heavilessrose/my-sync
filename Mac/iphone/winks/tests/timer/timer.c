#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
//#include <CarbonCore/OSUtils.h>
void gettimer(struct itimerval * tv){
	getitimer(ITIMER_REAL, tv);
}

void timerinfo(){
	struct itimerval tv;
	gettimer(&tv);
	printf("tv.it_value.tv_sec: %d\n", tv.it_value.tv_sec);
	printf("tv.it_value.tv_usec: %d\n", tv.it_value.tv_usec);
	printf("tv.it_interval.tv_sec: %d\n", tv.it_interval.tv_sec);
	printf("tv.it_interval.tv_usec: %d\n", tv.it_interval.tv_usec);
	printf("------------\n");
}

int n = 0;
void timefunc(int sig) /* 定时事件代码 */
{
	//fprintf(stderr, "ITIMER_PROF[%d]\n", n++);
	printf("ITIMER_PROF[%d]\n", n++);
	//signal(SIGPROF, timefunc); /* 捕获定时信号 */
	
	timerinfo();
}

void setnewtimer(){
	struct itimerval value;
	value.it_value.tv_sec=0; /* 定时1.5秒 */
	value.it_value.tv_usec=50000;
	value.it_interval.tv_sec=3; /* 重新开始1.5秒 */
	value.it_interval.tv_usec=50000;
	signal(SIGALRM, timefunc); /* 捕获定时信号 */
	setitimer(ITIMER_REAL, &value, NULL); /* 定时开始 */
}

void* testTimer(void *param){
	printf("----new thread\n");
	struct itimerval value;
	value.it_value.tv_sec=0; /* 定时1.5秒 */
	value.it_value.tv_usec=50000;
	value.it_interval.tv_sec=1; /* 重新开始1.5秒 */
	value.it_interval.tv_usec=50000;
	signal(SIGALRM, timefunc); /* 捕获定时信号 */
	setitimer(ITIMER_REAL, &value, NULL); /* 定时开始 */
	
	int i;
	for(i = 0; i < 10; i++){
		printf("-------new thread\n");
	}
	return NULL;
}


void stoptimer(){
	struct itimerval value;
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;
	if(setitimer(ITIMER_REAL, &value, NULL) == -1)
	{
		perror("stoptimer");
	}
	printf("stoptimer call end\n");
}


int main(int argc, const char* argv[])
{
//	testTimer(NULL);
//	
	pthread_t thread;
	pthread_create(&thread, NULL, &testTimer, NULL);
	timerinfo();
	sleep(5);
	timerinfo();
	// while(1);
	int i;
	for(i = 0; i < 10; i++){
		printf("main thread\n");
	}
	setnewtimer();
//	sleep(5);
	printf("-----\n");
	

	while(1){
	}
	stoptimer();
	return 0;
}

