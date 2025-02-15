#include <stdio.h>

#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

static pthread_t posixThreadID;
//static int flag = 0;
// 条件变量
static pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t condLock = PTHREAD_MUTEX_INITIALIZER;
static struct timespec timeout;
#define WAIT 1
#define RESUME 0
int ifWait = 0;


static int Winks_SetEvent(int event)
{
	printf("seting event %d\n", event);
	pthread_mutex_lock(&condLock);
	printf("got mutex \n");
	ifWait = event;
	pthread_mutex_unlock(&condLock);
	if(event == RESUME){
		printf("setevent: %s\n", "RESUME");
		pthread_cond_signal(&condition);
	}
	
    return 0;
}

static int Winks_GetEvent(struct timespec* timeout )
{
	pthread_mutex_lock(&condLock);
	while(WAIT == ifWait){
		printf("Winks_GetEvent: poll thread starting wait\n");
		pthread_cond_wait(&condition, &condLock);
		//pthread_cond_timedwait(&condition, &condLock, &timeout);
	}
	pthread_mutex_unlock(&condLock);
    return 0;
}

static int Winks_DeleteThread( void* thread ){
	
}

// The thread entry point routine.
void* PosixThreadMainRoutine(void* data)
{
	//	int oldstate;
	//	int oldtype;
	//	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstate);
	//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
	//	printf("oldstate: %d\n", oldstate);
	//	printf("oldtype: %d\n", oldtype);
	
	int n = 0;
	Winks_SetEvent(WAIT);
    printf( "WINKS socket thread initial wait\r\n" );
    Winks_GetEvent( &timeout );
    printf( "WINKS socket thread initial wakeup\r\n" );
	Winks_SetEvent(RESUME);
	
	int a = 1;
	while(1){
		//		if(flag)
		//			pthread_exit(NULL);
		sleep(1);
		while(a){
			printf("%d new posixThreadID: %u \n", n, (unsigned int)pthread_self());
			n++;
			if(n%5 == 0)
				a = 0;
		}
		if(!a){
			a = 1;
			Winks_SetEvent(WAIT);
            printf( "WINKS socket thread wait\r\n" );
            Winks_GetEvent( &timeout );
            printf( "WINKS socket thread wakeup\r\n" );
			Winks_SetEvent(RESUME);
		}
	}
	
	return NULL;
}

void LaunchThread()
{
	// Create the thread using POSIX routines.
	pthread_attr_t  attr;
	//   pthread_t       posixThreadID;
	
	assert(!pthread_attr_init(&attr));
	assert(!pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));
	
	int threadError = pthread_create(&posixThreadID, &attr, &PosixThreadMainRoutine, NULL);
	
	printf("main posixThreadID: %u \n", (unsigned int)pthread_self());
	assert(!pthread_attr_destroy(&attr));
	if (threadError != 0)
	{
		printf("threadError");
	}
	//	sleep(1);
}


int main(int argc, int *argv[]){
	LaunchThread();
	sleep(1);
	//	printf("---start cancel\n");
	//	pthread_testcancel();
	//	pthread_cancel(posixThreadID);
	////	flag = 1;
	//	printf("---canceled\n");
	
	int n = 0;
	while(1){
		sleep(2);
		printf("main sleeping\n");
		n++;
		if(n % 5 == 0){
			printf("start setevent: %s\n", "RESUME");
			Winks_SetEvent(RESUME);
		}
	}
}


//#include <unistd.h>
//#include <sys/types.h>
//#include <pthread.h>
//#include <stdio.h>
//#include <stdlib.h>
//typedef struct _linknode
//	{
//		int data;
//		struct _linknode * next;
//	}linkNode;
//
//typedef struct _linklist
//	{
//		int length;
//		linkNode * first;
//		pthread_mutex_t list_lock;
//	}linkList;
//
//typedef linkNode * LinkNodePtr;
//typedef linkList * LinkListPtr;
//
//typedef struct param
//	{
//		LinkListPtr list;
//		int * dataArray;
//		int number;
//	}Param;
//
//void outputLinkList(LinkListPtr list);
//
//void initLinkList(LinkListPtr list)
//{
//	if( pthread_mutex_init( &list->list_lock,NULL) != 0)
//		printf("can't get mutex_lock\n");
//	else
//		printf("succeed init mutex_lock\n");
//	
//	if(pthread_mutex_lock(&list->list_lock) != 0)
//		printf("init:can't lock\n");
//	list->length = 0;
//	list->first = NULL; 
//	if(pthread_mutex_unlock(&list->list_lock) != 0)
//		printf("init:can't unlock\n");
//}
//
//void insertLinkList(LinkListPtr list ,int data)
//{
//	if(pthread_mutex_lock(&list->list_lock) != 0)
//		printf("insertLinkList::can't lock");
//	LinkNodePtr  tempLinkNodePtr = NULL;
//	LinkNodePtr  currLinkNodePtr = (LinkNodePtr)calloc(1,sizeof(linkNode));
//	currLinkNodePtr->data = data;
//	currLinkNodePtr -> next = NULL;
//	if(list->length == 0)
//	{
//		list->first = currLinkNodePtr;
//		list->length = 1;
//	}
//	else
//	{
//		tempLinkNodePtr = list->first;
//		while(tempLinkNodePtr->next != NULL)
//		{
//			tempLinkNodePtr = tempLinkNodePtr ->next;
//		}
//		tempLinkNodePtr->next = currLinkNodePtr;
//		list->length += 1;
//	}
//	outputLinkList(list);
//	if( pthread_mutex_unlock(&list->list_lock) != 0)
//		printf("insertLinkList::can't unlock");
//}
//
//int removeLinkList(LinkListPtr list,LinkNodePtr node)
//{
//	pthread_mutex_lock(&list->list_lock);
//	LinkNodePtr preLinkNodePtr  = NULL;
//	LinkNodePtr currLinkNodePtr = NULL;
//	
//	if(list == NULL)
//		return -1;
//	preLinkNodePtr = list->first;
//	currLinkNodePtr = list->first;
//	
//	while( currLinkNodePtr != NULL && currLinkNodePtr != node)
//	{
//		preLinkNodePtr = currLinkNodePtr;
//		currLinkNodePtr = currLinkNodePtr->next; 
//	} 
//	if(currLinkNodePtr == NULL)//not exist node
//		return -1;
//	preLinkNodePtr->next = currLinkNodePtr->next;
//	free(currLinkNodePtr);
//	currLinkNodePtr = NULL;
//	pthread_mutex_unlock(&list->list_lock);
//	return 0; 
//}
//
//void insertLinkListArray(LinkListPtr currLinkListPtr,int dataArray[],int number)
//{
//	int i = 0; 
//	while(i<number)
//	{
//		insertLinkList(currLinkListPtr,dataArray[i++]);
//		
//	}
//}
//
//int destroyLinkList(LinkListPtr list)
//{
//	pthread_mutex_lock(&list->list_lock);
//	LinkNodePtr nextLinkNodePtr = NULL;
//	LinkNodePtr currLinkNodePtr = list->first;
//	while(currLinkNodePtr != NULL)
//	{
//		if(currLinkNodePtr->next != NULL)
//			nextLinkNodePtr = currLinkNodePtr->next;
//		else
//			nextLinkNodePtr = NULL;
//		free(currLinkNodePtr);
//		currLinkNodePtr = NULL;
//		if(nextLinkNodePtr != NULL)
//			currLinkNodePtr = nextLinkNodePtr;
//		else
//			break;
//	}
//	free(list);
//	list = NULL;
//	pthread_mutex_unlock(&list->list_lock);
//	return 0;
//}
//
//int searchLinkList(const LinkListPtr list,int data)
//{
//	LinkNodePtr currLinkNodePtr = list->first;
//	while(currLinkNodePtr != NULL)
//	{
//		if(currLinkNodePtr->data == data)
//		{
//			printf("%d exist\n",data);
//			return 0;
//		}
//		currLinkNodePtr = currLinkNodePtr->next;
//	}
//	printf("%d doesn't exist\n",data);
//	return -1;
//}
//
//void outputLinkList(LinkListPtr list)
//{
//	LinkNodePtr currLinkNodePtr = list->first;
//	while(currLinkNodePtr != NULL)
//	{
//		printf("%d-",currLinkNodePtr->data);
//		currLinkNodePtr = currLinkNodePtr->next;
//	}
//	printf("\n");
//}
//
//void * thread_call(void * arg)
//{
//	Param * param = (Param *)arg;
//	insertLinkListArray(param->list,param->dataArray,param->number);
//	pthread_exit( (void *)0);
//}
//
//int main()
//{
//	int *dataArray = (int *)calloc(10,sizeof(int));
//	int *dataArray2 = (int *)calloc(10,sizeof(int));
//	LinkListPtr list = (LinkListPtr)calloc(1,sizeof(linkList));
//    int i = 0;
//	pthread_t t_id1;
//	pthread_t t_id2;
//	Param tempParam1,tempParam2;
//	tempParam1.list = list;
//    tempParam1.dataArray = dataArray;
//	tempParam1.number = 10;
//	tempParam2.list = list;
//	tempParam2.dataArray = dataArray2;
//	tempParam2.number = 10;
//	
//	initLinkList(list);
//	for(i = 0 ; i <10;i++)
//	{
//		dataArray[i] = i;
//		dataArray2[i] = 10 + i;
//	}
//	if(pthread_create(&t_id1,NULL,thread_call,(void *)&tempParam1) != 0)
//		printf("thread create error");
//	if(pthread_create(&t_id2,NULL,thread_call,(void *)&tempParam2) != 0)
//		printf("thread create error");
//	
//	//list = createLinkList(list,dataArray,10);
//	sleep(5);//很重要,否则主线程可能在子线程自动之前就关闭了
//	searchLinkList(list,19);
//	searchLinkList(list,8);
//	//destroyLinkList(list);
//	
//	return 0;
//}