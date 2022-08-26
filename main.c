#include "SHAHash.h"
#include "ConvertFile.h"
#include <stdio.h>


int main(int argc, char *argv[]){
	//char fileName[] = "UploadFolder/sample_video.mp4";
	//FileToBlock(fileName);

	char fileName[] = "BlockFolder/sample_image.ablok";
	BlockToFile(fileName);
	
}
