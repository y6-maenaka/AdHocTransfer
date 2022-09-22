#include "TransferData.h"

void SendBlock(){
};



void ReceieBlock(){
	
	FormatRequestCommand();
};


RequestBlockCommand FormatRequestCommand( unsigned int position, unsigned char *flags){

	RequestBlockCommand command;
	memcpy( &command , &position, sizeof(unsigned int));
	memcpy( &command, &flags, sizeof(unsigned char));

	return command;
};
