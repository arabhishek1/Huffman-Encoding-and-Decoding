#include"pro.h"

void decode_file(char* file_decode)
{
	FILE *fp_read,*fp_write;
	array_t freq;
	int total_chars;
	unsigned char ch;	
	list_t L; 
	char h_codes[256][25];
	init_codes(h_codes);
	char code[25]="";	
	
	fp_write=fopen(file_decode,"w");
	fp_read=fopen("encoded.txt","r");
	
	fread(&freq,sizeof(array_t),1,fp_read);
	fread(&total_chars,sizeof(int),1,fp_read);
	
	printf("Totalchars: %d\n", total_chars);
	get_final_list( &L , &freq );
	
	treeptr tree=make_tree(&L);
	store_code(tree,code, h_codes);
	code[0]='\0';
	printf("Starting decoding\n");
	
	while(total_chars>0)
	{
		
		ch=fgetc(fp_read);
		//total_chars--;
		unsigned char local_var;
		unsigned char temp[25];	
		int end=0;
		
		get_code(temp,ch);
		strcat(code,temp);
		
		while( !end )
		{
			local_var=find_char(code,h_codes);
			Lshift_char( code  , strlen( h_codes[local_var] ) );
			end = (local_var == 255);
			if(!end)
			{ 
				fputc( local_var , fp_write );
				//printf("%c",local_var);
				total_chars--;
			}
		}	
		
	}
	printf("Ending\n");
	fclose(fp_read);
	fclose(fp_write);

}

unsigned char find_char(char* code, char h_codes[256][25])
{
	char local_var[25]="";
	int i=0,j=0;
	
	for(i=0 ; i<strlen(code) ; ++i)
	{	
		local_var[i]=code[i];
		local_var[i+1]='\0';
		for(j=0; j<256; j++)
		{
			if(! strcmp( local_var , h_codes[j] ) )
			return j;
		}
	}
	return 255;		//we havent returned the character	
}

//This function gets the binary code of the char
void get_code( char code[] , char ch )
{
	unsigned int mask=1<<31; //now mask is 1000 0000 til 4 byte complete
	unsigned int n=ch;
	n<<=24;
	int i;
	for( i=0 ; i<8 ; ++i )
	{
		if(n & mask)
		 code[i]='1';
		else
		 code[i]='0';
		n<<=1; //Leftshifting the value by one on the left
	}
	code[i]='\0'; //To make it a string 
} 
