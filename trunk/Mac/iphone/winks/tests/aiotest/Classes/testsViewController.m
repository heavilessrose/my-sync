//
//  testsViewController.m
//  tests
//
//  Created by luke on 7/11/09.
//  Copyright luke 2009. All rights reserved.
//

#import "testsViewController.h"

#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <aio.h>
#include <string.h>


#define TNAME "test.c"
#define PTS_PASS        0
#define PTS_FAIL        1
#define PTS_UNRESOLVED  2
#define PTS_UNSUPPORTED 4
#define PTS_UNTESTED    5

int testPosix();
@implementation testsViewController
@synthesize info;

char infoStr[2048];
int testAio(){
	int fd; 
	int ret; 
	char buf[1024]; 
	struct aiocb my_aiocb; 
//#ifdef TARGET_IPHONE_SIMULATOR
	strcat(infoStr, "TARGET_IPHONE_SIMULATOR\n");
	fd = open( "/Users/lukeluke/Desktop/tests/Classes/testsAppDelegate.h", O_RDONLY ); 
//#else
//	strcat(infoStr, "iphone device\n");
//	fd = open( "/private/var/root/iCosta.txt", O_RDONLY ); 
//#endif
	if (fd < 0) {
		perror("open");
		strcat(infoStr, strcat(strerror(fd), "\n"));
	}
	
	//Set up the control block 
	my_aiocb.aio_buf    = buf;
	my_aiocb.aio_fildes = fd;
	my_aiocb.aio_nbytes = sizeof(buf); 
	my_aiocb.aio_offset = 0; 
	
	ret = aio_read(&my_aiocb); 
	if (ret < 0) {
		strcat(infoStr, "aio_read error");
	} else {
		strcat(infoStr, "suport AIO");
	}
	
	close(fd); 
	return 0; 
}


int testPosix()
{
	char tmp[2048];
	char tmpfname[256];
#define BUF_SIZE 111
	unsigned char buf[BUF_SIZE];
	unsigned char check[BUF_SIZE];
	int fd;
	struct aiocb aiocb;
	int i;
	
#if _POSIX_ASYNCHRONOUS_IO != 200112L
	printf("_POSIX_ASYNCHRONOUS_IO = %d\n", _POSIX_ASYNCHRONOUS_IO);
	strcat(infoStr, "\nAIO not supported\n");
	//exit(PTS_UNSUPPORTED);
	//return 0;
#endif
	printf("_POSIX_ASYNCHRONOUS_IO = %d\n", _POSIX_ASYNCHRONOUS_IO);
	sprintf(tmp, "_POSIX_ASYNCHRONOUS_IO = %d\n",_POSIX_ASYNCHRONOUS_IO);
	strcat(infoStr, tmp);
	
	
	snprintf(tmpfname, sizeof(tmpfname), "/tmp/pts_aio_read_5_1_%d",
			 getpid());
	unlink(tmpfname);
	fd = open(tmpfname, O_CREAT | O_RDWR | O_EXCL,
			  S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		perror("open");
		sprintf(tmp, TNAME " Error at open(): %s\n", strerror(errno));
		strcat(infoStr, tmp);
		//exit(PTS_UNRESOLVED);
		return 0;
	}
	
	//unlink(tmpfname);
	
	for (i = 0; i < BUF_SIZE; i++)
		buf[i] = i;
	
	if (write(fd, buf, BUF_SIZE) != BUF_SIZE)
	{
		perror("write");
		sprintf(tmp, TNAME " Error at write(): %s\n", strerror(errno));
		//exit(PTS_UNRESOLVED);
		return 0;
	}
	
	memset(check, 0xaa, BUF_SIZE);
	memset(&aiocb, 0, sizeof(struct aiocb));
	aiocb.aio_fildes = fd;
	aiocb.aio_buf = check;
	aiocb.aio_nbytes = BUF_SIZE;
	aiocb.aio_lio_opcode = LIO_WRITE;
	
	if (aio_read(&aiocb) == -1)
	{
		perror("aio_read");
		sprintf(tmp, TNAME " Error at aio_read(): %s\n", strerror(errno));
		strcat(infoStr, tmp);
		//exit(PTS_FAIL);
		return 0;
	}
	
	int err;
	int ret;
	
	/* Wait until end of transaction */
	while ((err = aio_error (&aiocb)) == EINPROGRESS);
	
	err = aio_error(&aiocb);
	ret = aio_return(&aiocb);
	
	if (err != 0)
	{
		perror("aio_error");
		sprintf(tmp, TNAME " Error at aio_error() : %s\n", strerror (err));
		strcat(infoStr, tmp);
		close(fd);
		//exit(PTS_FAIL);
		return 0;
	}
	
	if (ret != BUF_SIZE)
	{
		sprintf(tmp, TNAME " Error at aio_return()\n");
		strcat(infoStr, tmp);
		close(fd);
		//exit(PTS_FAIL);
		return 0;
	}
	
	/* check it */
	
	for (i = 0; i < BUF_SIZE; i++)
	{
		if (buf[i] != check[i])
		{
			printf("check error\n");
			sprintf(tmp, TNAME " read values are corrupted\n");
			strcat(infoStr, tmp);
			//exit(PTS_FAIL);
			return 0;
		}
	}
	
	close(fd);
	sprintf(tmp, "Test PASSED\n");
	strcat(infoStr, tmp);
	return PTS_PASS;
}

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	//testAio();
	testPosix();
	[info setText:[NSString stringWithCString:infoStr]];
    [super viewDidLoad];
}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


- (void)dealloc {
    [super dealloc];
}

@end
