#include <stdio.h>
#include <stdlib.h>
#include "XImage.h"

int main (void){
	int error = 0;
	char InputFileName[256];
	char OutputFileName[256];
	XImage ximg;

	printf("Input Image File ===>");
	scanf("%s",InputFileName);

	error = ximg.ReadImage(InputFileName);
	
	printf("ReturnCode==>%d\n",error);


	// error = ximg.InverseImage();
	// error = ximg.MakeHistgramImage();
	// error = ximg.LinerTransformation();
	error = ximg.SmoothImage(16);
	printf("Output Image File ===>");
	scanf("%s",OutputFileName);

	error = ximg.SaveImage(OutputFileName);
	printf("ReturnCode==>%d\n",error);
	return error;
	
}
