#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <time.h> 
#include <errno.h> 
#include <aio.h> 

int main(int argc, char * argv[]) 
{ 
	int fd; 
	int ret; 
	char buf[1024]; 
	struct aiocb my_aiocb; 
	fd = open( "/Users/lukeluke/Desktop/aio/main.c", O_RDONLY ); 
	if (fd < 0) perror("open"); 
	
	//Set up the control block 
	my_aiocb.aio_buf    = buf; 
	my_aiocb.aio_fildes = fd; 
	my_aiocb.aio_nbytes = sizeof(buf); 
	my_aiocb.aio_offset = 0; 
	
	ret = aio_read(&my_aiocb); 
	if (ret < 0) perror("aio_read"); 
	
	close(fd); 
	return 0; 
} 