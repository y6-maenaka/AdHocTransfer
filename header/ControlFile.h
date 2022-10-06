#ifndef _H_ControlFile_
#define _H_ControlFile_

#include <stdio.h>
#include <uuid/uuid.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


long int GetFileSize(FILE *fp);
void GenerateUUID(uuid_t uuid);
void GenerateUUIDString(char *uuidFileNameString);
void LoadConfig();

char *GetAESKey();


#endif // _H_ControlFile_
