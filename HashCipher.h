#ifndef _H_HashCipher_
#define _H_HashCipher_

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/pem.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIGEST_NAME_SHA256 "sha256"
#define DIGEST_NAME_SHA512 "sha512"
#define RSA_KEY_SIZE 4096
#define PUBLIC_KEY_PEM_PATH ".config/.AhPublicKey.pem"
#define PRIVATE_KEY_PEM_PATH ".config/.AhPrivateKey.pem"

#define AES_BLOCK_SIZE 16 // BLOCK = PADDING

int sha512Hash(char *plainContext, unsigned char *digestMessage, unsigned int plainContextSize);

int AESEncrypt(const char *key, const char *encryptData, const size_t encryptDataLength, const unsigned char *iv, unsigned char *dest);

int AESDecrypt(const char *key, const unsigned char *decryptData, const size_t decryptDataLength, const unsigned char *iv, char *dest);

unsigned int GetAESEncryptedDataSize(size_t fileSize);

void GenerateRSAPrivateKey(EVP_PKEY *pkey);
void SaveRSAPrivateKey(EVP_PKEY *pkey);
void SaveRSAPublicKey(EVP_PKEY *pkey);
EVP_PKEY *ReadRSAPrivateKey();
EVP_PKEY *ReadRSAPublicKey();

unsigned char *RSAEncrypt(EVP_PKEY *pkey, unsigned char *plaintMess, size_t plainMessLength, size_t *outLength);

unsigned char *RSADecrypt(EVP_PKEY *pkey, unsigned char *encryptedMess, size_t encryptedMessLength, size_t *outLength);

void FreeEVPResource(EVP_PKEY *pkey, EVP_PKEY_CTX *pctx);

void CryptoError(char *errorMessage);


// dest : destination

#endif // _H_HashCipher_
