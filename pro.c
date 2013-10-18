#include"pro.h"

void encode_file(char* filename)
{
	array_t freq;
	char h_codes[256][25];
	char code[25]="";
	init_codes(h_codes);
	FILE* fp=fopen(filename,"r");		//opening a file in read mode 
	assert(fp!=NULL);		
	int total_chars=count_freq(&freq,filename);	//total contains the total number of chars in the file including repeated characters.
	fclose(fp);	
	list_t L;
	get_final_list( &L , &freq );
	
	treeptr tree=make_tree(&L);
	store_code(tree,code, h_codes);
	printf("Total no:%d\n", total_chars);
	compress(&freq, filename, h_codes ,total_chars);
	
	 
}



 int count_freq(array_t* a, char* filename)
{	
	int total=0;
	FILE *fp;
	fp=fopen(filename,"r");
	int i;
	for( i=0;i<256;++i)	//clearing every element to 0 at start.
	{
		a->myarray[i]=0;
	}
	
	char ch;
	
	while((ch=fgetc(fp))!=EOF)
	{
		a->myarray[ch]++;
		++total;		
	}
	fclose(fp);
	return total;
	
}

treeptr make_tree(list_t *l)
{
    if(l->head==0)
    {
        printf("\nEmpty file cannot be compressed\n");
    }
    else
          {
            while(l->head->next)
            {
            node_t *temp1=l->head;
		
            node_t *temp2=temp1->next;
		
            node_t *left=(node_t*)malloc(sizeof(node_t));
            left->llink=left->rlink=left->next=0;
            left->frequency=temp1->frequency;
            left->ch=temp1->ch;
            left->llink=temp1->llink; left->rlink=temp1->rlink;
            

             node_t *right=(node_t*)malloc(sizeof(node_t));
            right->llink=right->rlink=right->next=0;
            right->frequency=temp2->frequency;
            right->ch=temp2->ch;
            right->llink=temp2->llink; right->rlink=temp2->rlink;
            

            l->head=temp2->next;
            free(temp2);

            temp1->frequency=right->frequency+left->frequency;
            temp1->rlink=right;
            temp1->llink=left;
            temp1->ch=-1;
            insert_list(l,temp1);
            sort_list(l);
            }
    }
    return l->head;
}

void insert_list(list_t *l , node_t *temp)
{
    //We are only adding the nodes in the end.
    if(l->head==0)
    {
        l->head=temp;
        temp->next=0;
    }
    else
    {
        node_t *pres=l->head;
        node_t* prev=0;
        while(pres)
        {
            prev=pres;
            pres=pres->next;
        }
        prev->next=temp;
        temp->next=0;
    }
}

void get_final_list(list_t* ptr_list,array_t* a)
{
	create_list_array( a , ptr_list);
	sort_list(ptr_list);
    node_t* temp=ptr_list->head;
    while(temp->frequency==0)
    {
        node_t* hold=temp;
        ptr_list->head=hold->next;
        free(hold);
        temp=ptr_list->head;
    }
    
}

void sort_list(list_t *l)
{
    _Bool sorted=0;
    while(!sorted)
    {
        int i=0;
        node_t *x=l->head;
        node_t *prev=0;
        node_t *prev2=0;
        while(x)
        {
            prev2=prev;
            prev=x;
            x=x->next;
            if(!x);
            else if(x->frequency<prev->frequency)
            {
                i++;

                if(prev2==0)//swapping first two nodes
                {
                    prev->next=x->next;
                    x->next=prev;
                    l->head=x;
                }
                else //swapping in the middle and at the end
                {
                    prev->next=x->next;
                    x->next=prev;
                    prev2->next=x;
                }
            }
        }
        sorted=!i;
    }
}

void create_list_array( array_t *a , list_t *l)		//inserting the node in the beginning;
{
	l->head=0;
	int i=0;
	for( i=0;i<256;++i)
	{
		node_t *temp=(node_t *)malloc(sizeof(node_t));
		temp->llink=temp->rlink=temp->next=0;
		temp->frequency = a->myarray[i];
		temp->ch=i;
		
		temp->next=l->head;
		l->head=temp;
	}
}

void store_code(treeptr t , char code[], char codes[256][25])   //storing the path of all the nodes 
{
	char temp[25];
	
    if(t)
    {
     if(t->rlink==0 && t->llink==0)
        	{
        	strcpy(codes[t->ch],code);
		}
     if(t->llink!=0)
	{ 
		strcpy(temp,code);          
		strcat(temp,"0");
		store_code(t->llink,temp,codes);
	}
	if(t->rlink!=0)
		{
		strcpy(temp,code);
		strcat(temp,"1");
		store_code(t->rlink,temp,codes);
		}
	}
}

void init_codes(char h_codes[256][25])
{	
	int i;
	for(i=0;i<256;i++)
	{
		strcpy(h_codes[i],"");
	}
}

void compress(array_t *freq, char* filename, char codes[256][25], int total_chars)
{
	FILE *fp_read, *fp_write;
	unsigned char ch;
	char temp[25]="";
	
	fp_write=fopen("encoded.txt","w");
	fp_read=fopen(filename,"r");

	fwrite(freq,sizeof(*freq),1,fp_write);		//it writes the array containing the frequencies of all the chars into the encoded file.
	fwrite(&total_chars,sizeof(int),1,fp_write);	//it writes the total number of chars in the original file. 	
	//while((ch=fgetc(fp_read))!=EOF)
	while( total_chars )
	{
		ch=fgetc(fp_read);
		total_chars--;
		strcat(temp,codes[ch]);
				
		if(strlen(temp)>=8)
		{
			unsigned char local_var = get_ascii(temp);
			Lshift_char(temp,8);
			fwrite( &local_var , sizeof(local_var) , 1 , fp_write);
			//printf("%d",ch);
			// printf("i am repeating\n");
		
		}
		printf("%c",ch);
		//printf("strlen of temp: %d\n",strlen(temp));		
			
	}
	//#if 0
	int n=strlen(temp);
	while(n>0)
	{
		int i;
		//put the values of the remaining chars as '0'
		for( i=7 ; i>=n ; --i)
		temp[i]='0';
		//Make it as a string
		temp[8]='\0';
			unsigned char local_var=get_ascii(temp);
			Lshift_char(temp,8);
			fwrite( &local_var , sizeof(local_var) , 1 , fp_write);
			n=strlen(temp);
	}
	//#endif

	fclose(fp_read);
	fclose(fp_write);
}

int get_ascii( char byte[] )
{
	//We have to take the 8 chars and find its ascii equivalent and write that into the file.
	int f=0 , i; 	//for storing the ascii value
	for(i=0 ; i<8 ; ++i)
	{
		//take the corresponding char and multiply it with its weight
		//add it to f
		f=f+power(2,7-i)*(byte[i]-48);
	}
	return f;
		
}

int power( int i , int j)
{
	if(j==0) return 1;
	else return (i*power(i,j-1));
}

void Lshift_char(char byte[] , int n)
{
	int j = strlen(byte);	
	int i;
    for( i=0 ; i<n ; i++)
    {

           //Shifting the character code.
		int k=0;
            while(byte[k]!='\0')
            {
                byte[k]=byte[k+1];
		++k;
            }

	//byte[i-n]=byte[i];
    }
	byte[j-i]='\0';
}

