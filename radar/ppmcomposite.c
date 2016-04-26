#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * ppmfile = "Bg_full.ppm";
char * hfile = "sv_radar.h";
char * compositefile = "RvcCompositePPM";
char * ppmcfg = "ppmcfg";
FILE *wpFile;
FILE *compositepFile;
static int fileCount = 0;
static int fileIndex[128];
static int headSize = 0;


int del_linebreak(char *str)
{
	char ch;
	while('\n' != *str && *str)
	{
		++str;
	}
	*str = '\0';
	return 0;
}

int composite(char *file)
{
	FILE *pFile;
	char header[20];
	int imgWidth, imgHeight;
	int x_offset = 0;
	int y_offset = 0;
	char *pimgArgbData_tmp = NULL;
	char *pimgArgbData_back = NULL;
	char *pimgArgbData_freeback = NULL;
	char fileName[100];
	char x_offset_c[100];
	char y_offset_c[100];
	int x = 0;
	int y = 0;
	int z = 0;

	/*get file name and img position*/
	while(':' != *file){
		fileName[x] = *(file);
		x++;
		++file;	
	}
	fileName[x] = '\0';	

	++file;
	while(',' != *file){
		x_offset_c[y] = *file;
		y++;
		++file;	
	}
	x_offset_c[y] = '\0';
	x_offset = atoi(x_offset_c);	
	
	++file;
	while('\0' != *file){
		y_offset_c[z] = *file;
		z++;
		++file;	
	}
	y_offset_c[z] = '\0';
	y_offset = atoi(y_offset_c);	

	printf("sv x is %d %s \n", x, &fileName[0]);
	printf("sv y is %d %s %d\n", y, &x_offset_c[0], x_offset);
	printf("sv z is %d %s %d\n", z, &y_offset_c[0], y_offset);

	pFile = fopen(fileName, "rb");
	if(pFile == NULL) {
  		printf("open file[%s] failed\n",fileName);
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
	pimgArgbData_freeback  = pimgArgbData_tmp;
	fread(pimgArgbData_tmp, imgWidth*imgHeight*4, 1, pFile);

	fileCount ++;
	fileIndex[fileCount] = fileIndex[fileCount-1] + 4*4 + imgWidth*imgHeight*4; 	
	printf("sv file index%d is %d \n", fileCount,fileIndex[fileCount]);
	
	fwrite(&imgWidth, 4, 1, compositepFile);
	fwrite(&imgHeight, 4, 1, compositepFile);
	fwrite(&x_offset, 4, 1, compositepFile);
	fwrite(&y_offset, 4, 1, compositepFile);
	fwrite(pimgArgbData_back, imgWidth*imgHeight*4, 1, compositepFile);

	free(pimgArgbData_freeback);
	fclose(pFile);
	return 1;
}


int main()
{
	char fileName[100];
	int ret = 0;
	int imgWidth, imgHeight;
	char *pimgArgbData_tmp = NULL;
	char *pimgArgbData_back = NULL;
	int i;
	unsigned char tmp;
	FILE *pCfgFile;

        compositepFile = fopen(compositefile, "wb");
        if( compositepFile == NULL) {
                printf("open file[%s] failed\n",compositefile);
                return 1;
        }

	pCfgFile = fopen(ppmcfg, "rb");
	if(pCfgFile  == NULL) {
  		printf("open file[%s] failed\n",ppmcfg);
		return 1;
	}

	/*clear head*/
	memset(&fileIndex,0,sizeof(fileIndex));
	headSize = sizeof(fileIndex);
	fwrite(fileIndex, headSize, 1, compositepFile);

	fileIndex[0] = headSize;
	printf("sv file index0 is %d \n", fileIndex[fileCount]);

	/*read ppm file name form cfg file and composite them*/
	do{
		if(fgets(fileName, 100, pCfgFile) != NULL && strstr(fileName,".ppm"))
		{
			del_linebreak(fileName);
			printf("sv get file cfg %s \n",fileName);
			composite(fileName);
		}
	}
	while(!feof(pCfgFile));

	fseek(compositepFile,0,SEEK_SET);
	fwrite(fileIndex, headSize, 1, compositepFile);

#if 0
	wpFile = fopen(hfile, "w+");
	if(wpFile == NULL) {
  		printf("open file[%s] failed\n",hfile);
		return 1;
	}

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

#endif	
//	composite(ppmfile);
	
	fclose(compositepFile);
	fclose(pCfgFile);

	return ret;

}
