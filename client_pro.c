#include "pro.h"

int main()
{
	char filename[25];
	printf("Enter the file to be compressed\n");
	scanf("%s", filename);
	encode_file(filename);

	printf("Enter the filename to which the encoded file has to be decoded\n");
	char file_decode[25];
	scanf("%s", file_decode);
	
	decode_file(file_decode);	

	return 0;
}	
