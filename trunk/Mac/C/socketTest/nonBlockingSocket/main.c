#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>

#include <assert.h>
#include <pthread.h>

#define host "www.google.com"
#define PORT "80"
static int ready = 0;

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void asyncSelect(int sockfd){
	struct timeval tv;
    fd_set readfds, writefds, exceptfds;
	
    tv.tv_sec = 30;
    tv.tv_usec = 500000;
	
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
	
    // don't care about writefds and exceptfds:
    select(sockfd+1, &readfds, &writefds, &exceptfds, &tv);
	ready = 1;
	
    if (FD_ISSET(sockfd, &readfds))
        printf("ready for read!\n");
    else
        printf("Timed out.\n");    
	if (FD_ISSET(sockfd, &readfds))
			printf("ready for write!\n");
		else
			printf("Timed out.\n");
	if (FD_ISSET(sockfd, &readfds))
        printf("ready for exceptfds!\n");
    else
        printf("Timed out.\n");
	
}

// The thread entry point routine.
void* PosixThreadMainRoutine(void* data)
{
	printf("new posixThreadID: %u \n", (unsigned int)pthread_self());
	int sock;
	memcpy(&sock, data, sizeof(int));
	printf("sock: %d\n", sock);
	asyncSelect(sock);
	
    return NULL;
}

void LaunchThread(int* sockfd)
{
    // Create the thread using POSIX routines.
    pthread_attr_t  attr;
    pthread_t       posixThreadID;
	
    assert(!pthread_attr_init(&attr));
    assert(!pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));
	
    int threadError = pthread_create(&posixThreadID, &attr, &PosixThreadMainRoutine, sockfd);
	
	printf("main posixThreadID: %u \n", (unsigned int)pthread_self());
    assert(!pthread_attr_destroy(&attr));
    if (threadError != 0)
    {
		printf("threadError\n");
    }
	//sleep(1);
}

int main (int argc, const char * argv[]) {
	//int sockfd = socket(PF_INET, SOCK_STREAM, 0);
//	fcntl(sockfd, F_SETFL, O_NONBLOCK); // setting a socket to non-blocking
//    printf("Hello, World!\n");
	
	
	int sockfd, rv;
	struct addrinfo hints, *servinfo, *p;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	// 取得服务器地址信息（struct addrinfo）
	if ((rv = getaddrinfo(host, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server socket"); // 创建socket描述符失败
			continue;
		}
		fcntl(sockfd, F_SETFL, O_NONBLOCK); // setting a socket to non-blocking

		break;
	}
	
	if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
		close(sockfd);
		perror("connect"); // connect失败
	}
	////////////////////
	printf("sockfd: %d\n", sockfd);
	LaunchThread(&sockfd);
	while(ready == 0){
		printf("waiting ..\n");
	}
	printf("done\n");
	sleep(1);
    return 0;
}
