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

typedef struct HashTableRec{
	BucketList hashTable[SIZE];
	int scope;
	
	struct HashTableRec *outer;
	struct HashTableRec *inner;
} * HashTableList;

//	The hash table
static HashTableList hashTableList = NULL;
static HashTableList curHashTable = NULL;
static int curScope = 0;

//	Hash table List initialiization
void initSymbolTable(void){
	hashTableList = (HashTableList)malloc(sizeof(struct HashTableRec));
	hashTableList->inner = NULL;
	hashTableList->outer = NULL;
	hashTableList->scope = 0;
	curHashTable = hashTableList;
}

//	Come into new scope
void intoNewScope(void){
	HashTableList temp = (HashTableList)malloc(sizeof(struct HashTableRec));
	temp->outer = curHashTable;
	temp->inner = curHashTable->inner;
	if (temp->inner)
		temp->inner->outer = temp;
	curHashTable->inner = temp;
	curHashTable = temp;
	curScope++;
	curHashTable->scope = curScope;
}

//      Go out the scope
void closeScope(void){
	hashTableList = hashTableList->outer;
	curScope--;
}

//	Insertion to hash table
int st_insert(char * name, int lineno, int scope, int loc, NodeKind kind, int subKind, int arrSize, TokenType type){
	/*int h = hash(name);
	BucketList l = hashTableList;

	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;

	if (l == NULL){
		if (kind == ExpK)
			return -1	//Scoping Error
		

		l->name = name;		//복사해서 줘야함??
		l->lines = (LineList)malloc(sizeof(struct LineListRec));
		l->lines->lineno = lineno;
		l->loc = loc;
		l->kind = kind;
		l->subKind = subKind;
		l->arrSize = arrSize;
		l->type = type;
	}
	else{
		if (subkind == K){
		}
		else{
		}
	}
	*/
	return 1;
}

int st_lookup(char * name){
	return 0;
}

void printSymTab(FILE *listing){
}
