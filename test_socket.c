#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>


int main(){
	int sock , tmp;
	int sockOptSize = 0;
	int rcvBufferSize = 0;
	sockOptSize = sizeof(rcvBufferSize);

	if(( sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		printf("Error happen\n");

	printf("%d\n", rcvBufferSize);

	//tmp = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcvBufferSize , &sockOptSize);

	puts("+++++++++++++++");
	//printf("%d",rcvBufferSize);

	struct sockaddr_in servAddr;

	puts("=========");
	printf("%lu",sizeof(servAddr.sin_addr.s_addr));


	
}
