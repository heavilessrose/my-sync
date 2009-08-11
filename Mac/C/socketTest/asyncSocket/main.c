#include <stdio.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>

static void sigio_func(int);
void err_sys(const char *fmt, ...);
static void err_doit(int errnoflag, const char *fmt, va_list ap);
int main (int argc, const char * argv[]) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	/*
	 * Should be able to set this with fcntl(O_ASYNC) or fcntl(FASYNC),
	 * but some systems (AIX?) only do it with ioctl().
	 *
	 * Need to set this for listening socket and for connected socket.
	 */
	signal(SIGIO, sigio_func);
	
	if (fcntl(sockfd, F_SETOWN, getpid()) < 0)
		err_sys("fcntl F_SETOWN error");
	
	if (fcntl(sockfd, F_SETLK, O_ASYNC) < 0)
		err_sys("fcntl O_ASYNC error");
//	int option = 1;
//	if (ioctl(sockfd, FIOASYNC, (char *) &option) < 0)
//		err_sys("ioctl FIOASYNC error");
}


static void
sigio_func(int signo) {
	fprintf(stderr, "SIGIO\n");
	/* shouldn't printf from a signal handler ... */
}

void
/* $f err_sys $ */
err_sys(const char *fmt, ...) {
	va_list		ap;
	
	va_start(ap, fmt);
	err_doit(1, fmt, ap);
	va_end(ap);
	exit(1);
}

static void err_doit(int errnoflag, const char *fmt, va_list ap) {
	int		errno_save;
	char	buf[1];
	
	errno_save = errno;		/* value caller might want printed */
	vsprintf(buf, fmt, ap);
	if (errnoflag)
		sprintf(buf+strlen(buf), ": %s", strerror(errno_save));
	strcat(buf, "\n");
	fflush(stdout);		/* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(stderr);		/* SunOS 4.1.* doesn't grok NULL argument */
	return;
}
