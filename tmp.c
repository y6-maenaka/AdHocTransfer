#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>

#include <stdlib.h>


#define DIGEST_NAME "sha512"

int main(){
	EVP_MD_CTX *mdctx;
	const EVP_MD *md;
	unsigned char md_value[EVP_MAX_MD_SIZE];
	int md_len;
	char mess[] = "Test\n";


	FILE *fp;
	char filename[] = "UploadFolder/sample_video.mp4";
	char buffer[10];
	//char buffer[64000];
	fp = fopen(filename,"r");

	fread(buffer,10,10,fp);
	printf("%s",buffer);
	

	fclose(fp);




	OpenSSL_add_all_digests();
	md = EVP_get_digestbyname(DIGEST_NAME);

	mdctx = EVP_MD_CTX_create();
	//EVP_DigestInit_ex(mdctx,md,NULL);
	EVP_DigestInit(mdctx,md);
	EVP_DigestUpdate(mdctx,mess,strlen(mess));
	EVP_DigestFinal_ex(mdctx,md_value,&md_len);
	EVP_MD_CTX_destroy(mdctx);

	for(int i=0; i < md_len; i++){
		printf("%02x",md_value[i]);
	}

	EVP_cleanup();
	exit(0);

}
