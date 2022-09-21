#include <stdio.h>

#define BLOCK_EXTENSION ".ablock"

struct sample{
	int test;
};

void change( struct sample *_a){
	_a->test = 10;
}

int main(){	
	//void *tmp;
	//tmp = calloc( 4194, 1 );


	//free( tmp );
	
	printf("%d", 9/8);

}
