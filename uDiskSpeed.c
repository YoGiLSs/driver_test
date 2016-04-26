#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int tim_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{
	int nsec;
	if ( x->tv_sec > y->tv_sec )
		return   -1;
	if ((x->tv_sec==y->tv_sec) && (x->tv_usec>y->tv_usec))
		return   -1;
	result->tv_sec = ( y->tv_sec-x->tv_sec );
	result->tv_usec = ( y->tv_usec-x->tv_usec );
	if (result->tv_usec<0)
	{
		result->tv_sec--;
		result->tv_usec+=1000000;
	}
	return	0;
}

int main()
{
	struct   timeval   start_read,stop_read,diff_read;

	/* drop cache */
	system("echo 3 > /proc/sys/vm/drop_caches");

	printf("read start\n");
	gettimeofday(&start_read,0);
	system("dd if=/dev/mmcblk1 of=/dev/null bs=1M count=256");
	gettimeofday(&stop_read,0);
	printf("read end\n");
	
	tim_subtract(&diff_read,&start_read,&stop_read);

	printf("uDisk read 256MB in %d.%d Second\n",  diff_read.tv_sec, diff_read.tv_usec/1000);
	printf("uDisk read speed:%fM/s\n", (double)(256*1000)/(diff_read.tv_sec*1000 + diff_read.tv_usec/1000));
	return 0;
}
