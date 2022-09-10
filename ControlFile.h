#ifndef _H_ControlFile_
#define _H_ControlFile_

#include <stdio.h>
#include <uuid/uuid.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define KEYSIZE 16

long int GetFileSize(FILE *fp);
void GenerateUUID(uuid_t uuid);
void GenerateUUIDString(uuid_string_t uuidFileNameString);
void LoadConfig();


#endif // _H_ControlFile_
