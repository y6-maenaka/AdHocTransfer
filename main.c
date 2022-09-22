#include "HashCipher.h"
#include "ControlFile.h"
#include "ControlConnection.h"
#include "ConvertFile.h"
#include "Sample.h"


int main(int argc, char *argv[]){
	LoadConfig();



	/*
	BlockKey key;
	//printf("%zu", sizeof(unsigned char));
	//printf("%zu", sizeof( BlockKey));
	void *blockKeyBuffer;
	blockKeyBuffer = calloc( sizeof(BlockKey), 1);

	FILE *block_fp = NULL;
	block_fp = fopen("KeyFolder/sample_video.mp4.akey", "rb");
	fread(blockKeyBuffer, sizeof(BlockKey), 1, block_fp);

	FormatBlockKey( blockKeyBuffer, &key);
	
	printf("originalFileSize -> %u\n", key.originalFileSize);
	printf("fileNum -> %s\n",key.fileName);
	printf("blockDigestMessage -> %s\n", key.blockDigestMessage);
	printf("blockName -> %s\n", key.blockName);

	unsigned char buf[1] = {0};

	for(int i=0; i<525; i++){
		memcpy( &buf, key.blockFlags + i, 1);
		printf("%d ", *buf);
	}



	fclose( block_fp );
	free(blockKeyBuffer);
	*/
	

	

/*
	EVP_PKEY *pkey = NULL;
	EVP_PKEY *_pkey = NULL;

	pkey = ReadRSAPublicKey();
	//EVP_PKEY_print_public_fp(stdout, pkey, 0 , NULL);
	puts("");
	memcpy(&_pkey, &pkey, 8);
	puts("=======");
	EVP_PKEY_print_public_fp(stdout, _pkey, 0 , NULL);
	*/

	/*	
	int sock;
	char servIP[11] = "127.0.0.1";
	unsigned short servPort = 8080;
	sock = ClientConnection(servIP, servPort);

	EVP_PKEY *pkey = NULL;
	pkey = ReadRSAPublicKey();
	int m = EVP_PKEY_size(pkey);
	
	//printf(" pkey size -> %d\n",m);
	//EVP_PKEY_print_public_fp(stdout, pkey, 0, NULL);
	sleep(1);
	//SendCommand(sock,  RSA_PUBLIC_KEY, sizeof(pkey), &pkey);
	//EVP_PKEY_print_public_fp(stdout, pkey, 0, NULL);
	//puts("send command done 1");

	int sendQSize, recvQSize;
	SetSocketQSize(sock, SOCKET_BASE_RECEIVEQ_SIZE, SOCKET_BASE_SENDQ_SIZE);
	GetSocketQSize(sock, &sendQSize, &recvQSize);
	puts("helo");
	sleep(2);
	SendCommand(sock, SOCKET_RECEIVEQ_SIZE, sizeof(recvQSize), &recvQSize);
	printf("recv Q size is -> %d\n", recvQSize);
	SendCommand(sock, SOCKET_RECEIVEQ_SIZE, sizeof(recvQSize), &recvQSize);
	puts("hello after");
	close(sock);
	*/

	/*	
	int sock;
	PeerInformation PeerInf;
	unsigned short servPort = 8080;
	int tmp = ServerConnection(servPort);
	sleep(11);
	*/

	//EVP_PKEY_print_public_fp( stdout, PeerInf.PeerRSAPublicKey, 0, NULL);
	//printf("%s", PeerInf.PeerRSAPublicKey);

	//char fileName[] = "sample_video.mp4";
	//FileToBlock(fileName);

	//char fileName[] = "sample_image.ablok";
	//BlockToFile(fileName);
	


	//AESEncryptFile("BlockFolder/%9003BF3B-AC66-4EEB-97EB-5009F331C624.ablock");
	//AESDecryptFile("BlockFolder/%9003BF3B-AC66-4EEB-97EB-5009F331C624.ablock");


	/*
	char key[16] = "abcdefghijklmnop";
	char cryptData[] = "hello, OpenSSL! 123456789012345";
	unsigned char iv[16] = "abcdefghijklmnop";
	char decodeData[64] = {'\0'};

	unsigned char *sample;
	char *_sample;

	//datasize = datasize + ( blocksize - (datasize % block_size))

	size_t dataSize = sizeof(cryptData) + ( AES_BLOCK_SIZE - (sizeof(cryptData) % AES_BLOCK_SIZE));



	sample = malloc(GetAESDecryptedDataSize(sizeof(cryptData)));
	_sample = malloc(dataSize);

	int tmp = AESEncrypt(key , cryptData, sizeof(cryptData),iv, sample);
	int _tmp = AESDecrypt(key, sample, dataSize, iv, _sample);


	for(int i=0; i<dataSize; i++) printf("%c",sample[i]);

	//printf("%s",_sample);

	free(sample);
	free(_sample);
	*/

	/*
	for(int i=0; i < sizeof(_u); i++)
		printf("%c",_u[i]);
	*/

	/*
	uuid_t uuid;
	GenerateUUID(uuid); // 128bit 32words


	FILE *config_fp;
	char key[sizeof(uuid)] = {0};

	memcpy(key, (char *)uuid, sizeof(uuid));

	if( (config_fp = fopen("config/.AdHocTransfer.aconf","rb")) ){
		fread(key, sizeof(uuid), 1, config_fp);
		printf("%s\n",key);

	}else{
		config_fp = fopen("config/.AdHocTransfer.aconf","wb");
		fwrite(key, sizeof(uuid), 1, config_fp );
	}

	fclose(config_fp);
	*/


	//LoadConfig();


	/* ======= RSA =============================

	EVP_PKEY *pkey = NULL;
	//GenerateRSAPrivateKey(pkey);

	unsigned char sampleMess[23] = "This is Sample Message";
	unsigned char *tmp;
	unsigned char *_tmp;

	size_t outLength, _outLength;


	pkey = ReadRSAPublicKey();
	tmp = RSAEncrypt(pkey, sampleMess, sizeof(sampleMess), &outLength);



	printf("%s\n",tmp);

	puts("+++++++++++++++==");
	EVP_PKEY *_pkey = NULL;
	_pkey = ReadRSAPrivateKey();
	_tmp = RSADecrypt(_pkey, tmp, outLength, &_outLength );

	printf("%s",_tmp);

	OPENSSL_free(tmp);

	============================================================ */
}
