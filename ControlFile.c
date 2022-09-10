#include "ControlFile.h"

long int GetFileSize(FILE *fp){

	unsigned int fileSize = -1;

	fseek(fp,0,SEEK_END);
	fileSize = ftell(fp);
	fseek(fp,0,SEEK_SET);

	if ( fileSize <= 0 ){
		fprintf(stderr, "can not get file size");
		exit(1);
	}

	return fileSize;
}

void GenerateUUID(uuid_t uuid){
	uuid_generate(uuid);
}


void GenerateUUIDString(uuid_string_t uuidFileNameString){

	uuid_t uuidFileName;
	uuid_generate(uuidFileName);
	uuid_unparse_upper(uuidFileName, uuidFileNameString);

}

void LoadConfig(){

	FILE *config_fp;
	char key[KEYSIZE] = {0};

	srand((unsigned int)time(NULL));

	if ( (config_fp = fopen(".config/.AdHocTransfer.aconf", "rb")) ){
		fread( key, KEYSIZE, 1, config_fp );
	}else{

		config_fp = fopen(".config/.AdHocTransfer.aconf","wb");

		for(int i=0; i<sizeof(key); i++)
			key[i] = rand() % 126 + 33;

			fwrite( key, KEYSIZE, 1, config_fp );
	}

	fclose(config_fp);

}
