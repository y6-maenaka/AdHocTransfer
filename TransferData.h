#ifndef _H_TransferData_
#define _H_TransferData_

typedef struct REQUEST_BLOCK_COMMAND{
	unsigned int  position;
	unsigned char flags[1];
}RequestBlockCommand;




void SendBlock();
void ReceiveBlock();

RequestBlockCommand FormatRequestCommand( unsigned int position, unsigned char *flags);


#endif // _H_TransferData_
