#include "apue.h"
#include "www.h"

int		glob = 6;		/* external variable in initialized data */
static char	buf[] = "a write to stdout\n";

int
main(void)
{
	int		var;		/* automatic variable on the stack */
	pid_t	pid;
	
	var = 88;
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
		err_sys("write error");
	printf("before fork\n");	/* we don't flush stdout */
	
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {		/* child:子进程返回0 */
		glob++;					/* 子进程改变变量：父，子进程不共享数据段 */
		var++;
	} else {
		// 父进程睡眠2秒，使子进程先运行
		sleep(2);				/* parent:父进程返回子进程的pid */
	}
	// 父，子进程共享正文段
	printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
	pppp();
	exit(0);
}
