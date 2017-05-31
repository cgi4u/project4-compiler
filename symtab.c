#include <stdio.h>
#include <string.h>
#include "symtab.h"

//Constants used to control hash table
#define SIZE 211
#define SHIFT 4

//	hash function
static int hash (char * key){
	int temp = 0;
	int i = 0;
	while (key[i] != '\0'){
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

//	The list of line numbers for a symbol
typedef struct LineListRec{
	int lineno;

	struct LineListRec *next;
} * LineList;

//	Bucket list for each symbol. Containing its properties
typedef struct BucketListRec{
	char *name;
	LineList lineList;
	int loc;
	NodeKind kind;
	int subKind;
	int arrSize;
	TokenType type;

	struct BucketListRec *next;
} * BucketList;

//	The hash table
static BucketList hashTable[SIZE];

void st_insert(char * name, int lineno, int scope, int loc, NodeKind kind, int subKind, int arrSize, TokenType type){
}

int st_lookup(char * name){
	return 0;
}

void printSymTab(FILE *listing){
}
