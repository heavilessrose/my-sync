//#include "apue.h"
//
//static void my_exit1(void);
//static void my_exit2(void);
//int main (int argc, const char * argv[]) {
//	if(atexit(my_exit1) != 0)
//		err_sys("can't register my_exit1");
//	if(atexit(my_exit2) != 0)
//		err_sys("can't register my_exit2");
//	if(atexit(my_exit1) != 0)
//		err_sys("can't register my_exit1");
//	
//	printf("main is done\n");
//    return 0;
//}
//
//
//static void my_exit1(void){
//	printf("first exit handler\n");
//}
//
//static void my_exit2(void){
//	printf("second exit handler\n");
//}


//#include "apue.h"
//
//int
//main(int argc, char *argv[])
//{
//	int		i;
//	
//	for (i = 0; /*i < argc*/ argv[i] != NULL; i++)		/* echo all command-line args */
//		printf("argv[%d]: %s\n", i, argv[i]);
//	exit(0);
//}
