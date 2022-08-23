#include "SHAHash.h"

// EVP_MAX_MD_SIZE = 64

int sha256Hash(char *plainContext, unsigned char *digestMessage){	
	EVP_MD_CTX *mdctx; // メッセージダイジェストの状態を管理する構造体
	const EVP_MD *md; // メッセージダイジェスト関数を指定する構造体
	unsigned int mdLength;

	OpenSSL_add_all_digests(); // Setting internal table (internal structure)
														
	md = EVP_get_digestbyname(DIGEST_NAME_SHA256); // use SHA256 function(algorithm)
																								 
	if ( !md ){
		fprintf(stderr,"Digest name %s is incorrect",DIGEST_NAME_SHA256);
		exit(1);
	}

	mdctx = EVP_MD_CTX_create(); // create digest context and initialize
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, plainContext, strlen(plainContext));
	EVP_DigestFinal_ex(mdctx, digestMessage, &mdLength); // mdLength is size of bytes

	EVP_MD_CTX_destroy(mdctx);
	EVP_cleanup();


	/*
	for(int i=0;i<mdLength;i++)
		printf("%02x\n",digestMessage[i]);

	printf("%d",mdLength);
	*/
	

	return mdLength;
}
