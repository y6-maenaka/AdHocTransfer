#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void SIGReceive(){
	puts("receive signal");
}


int main(){
	/*
	struct sigaction handler;
	handler.sa_handler = SIGReceive;
	handler.sa_flags = 0;
	sigfillset( &handler.sa_mask );
	sigaction(SIGALRM, &handler, 0);

	alarm( 3 );

	for(;;){
		puts("sleeping now");
		sleep(2);
	}
	*/
	printf("%d", 65/8);
}
