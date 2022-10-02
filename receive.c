#include "HashCipher.h"
#include "ControlFile.h"
#include "ControlConnection.h"
#include "ConvertFile.h"
#include "TransferData.h"

int main(int argc, char *argv[]){


	LoadConfig();

	//ServerConnection();
	ReceiveBlock();
};
