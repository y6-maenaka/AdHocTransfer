#ifndef _H_SHAHash_
#define _H_SHAHash_

#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>

#define DIGEST_NAME_SHA256 "sha256"

int sha256Hash(char *plainContext, unsigned char *digestMessage);

#endif // _H_SHAHash_
