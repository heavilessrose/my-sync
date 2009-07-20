#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

int main (int argc, const char * argv[]) {

	uint64_t start;
    uint64_t end;
	uint64_t elapsed;
	start = mach_absolute_time();
	end = mach_absolute_time();
	elapsed = end - start;
	printf("elapsed = %d\n", elapsed);
	
	
	clock_t clocks = clock();
	printf("clock() ＝ %d\n", clocks);
	//////
	
	struct timeval time;
	struct timezone timeZone;
	
	gettimeofday(&time, &timeZone);
	
	printf("time: \n");
	
	
    return 0;
}
