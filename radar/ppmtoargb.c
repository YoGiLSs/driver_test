#include <stdio.h>
#include <stdlib.h>

char * ppmfile = "Bg_full.ppm";
char * wfile = "sv_radar.h";

int main()
{
	char header[20];
	FILE *pFile;
	FILE *wpFile;
	int ret = 0;
	int imgWidth, imgHeight;
	char *pimgArgbData_tmp = NULL;
	char *pimgArgbData_back = NULL;
	int i;
	unsigned char tmp;

	pFile = fopen(ppmfile, "rb");
	if(pFile == NULL) {
  		printf("open file[%s] failed\n",ppmfile);
		return 1;
	}

	wpFile = fopen(wfile, "w+");
	if(wpFile == NULL) {
  		printf("open file[%s] failed\n",ppmfile);
		return 1;
	}
	// get "P6"
	fgets(header, 20, pFile);

	// get "width height"
	fgets(header, 20, pFile);
	sscanf(header, "%d %d\n", &imgWidth, &imgHeight);
	printf("imgWidth is %d, imgHeight is %d \n",imgWidth,imgHeight);

	// get "255"
	fgets(header, 20, pFile);

	// get rgb data
	pimgArgbData_tmp = calloc(1,imgWidth*imgHeight*4);
	pimgArgbData_back = pimgArgbData_tmp;
	fread(pimgArgbData_tmp, imgWidth*imgHeight*4, 1, pFile);

	
	fprintf(wpFile,"\n "
			"static unsigned char sv_radar_data[] __initdata = { \n");

	fprintf(wpFile,"\t");
	for(i = 0; i < imgWidth*imgHeight*4; i++)
	{
	//	if(i%4 == 0)
			tmp = *((char *)pimgArgbData_back+i);
		fprintf(wpFile,"0x%02X,",tmp);
		if(i%8 == 7)
			fprintf(wpFile,"\n\t");
				
	}


	fprintf(wpFile," \n}; \n");

	fclose(pFile);

	return ret;
}
