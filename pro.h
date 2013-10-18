#ifndef HUFFMAN
#define HUFFMAN

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
struct node
{
    char ch;    		//May be any char including special and non printable chars
                     		// They are stored in their ascii values in the memory.
    int frequency;  		//Stores the Frequency of occurence in the file
    struct node *llink;		//llink and rlink act as links for LST and RST
    struct node *rlink;
    struct node *next;		//next link is used to point to next node in the linked list
};
typedef struct node node_t;
typedef node_t* treeptr;

struct list
{
    node_t *head;
};
typedef struct list list_t;

struct data
{
	int frequency;
	unsigned char ch;
};
typedef struct data data_t;

struct array			//structure of array to store the frequency of the characters corresponding to their frequencies.
{
	int myarray[256];	//256 unsigned ascii characters.
};
typedef struct array array_t;

void encode_file(char* filename);
void decode_file(char* file_decode);
int count_freq(array_t* a, char* filename);
void get_final_list(list_t* ptr_list,array_t* a);
void sort_list(list_t *l);
void create_list_array( array_t *a , list_t *l);
void store_code(treeptr t , char code[], char codes[256][25]);
void init_codes(char h_codes[256][25]);
void compress(array_t *freq, char* filename, char codes[256][25], int total_chars);
void insert_list(list_t *l , node_t *temp);
treeptr make_tree(list_t *l);
int get_ascii( char byte[] );
int power( int i , int j);
void Lshift_char(char byte[] , int n);

unsigned char find_char(char* code, char h_codes[256][25]);
void get_code( char code[] , char ch );

#endif
