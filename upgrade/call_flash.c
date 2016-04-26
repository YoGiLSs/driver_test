#include <stdio.h>
#include <stdlib.h>


int main()
{
	printf("yogi test start \n");
	system("bash /media/usbstorage0/qspi_burn_nor_tool_20150825/qspi_burn_nor_local.sh freertos-ind-a7boot-output.bin");
	printf("end \n");

	return 0;

}

