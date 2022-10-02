#include "header/HashCipher.h"
#include "header/ControlFile.h"
#include "header/ControlConnection.h"
#include "header/ConvertFile.h"
#include "header/TransferData.h"

int main(int argc, char *argv[]){


	LoadConfig();

	//ServerConnection();
	ReceiveBlock();
};
