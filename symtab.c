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
	LineList lines;
	int loc;
	NodeKind kind;
	int subKind;
	int arrSize;
	ExpType type;

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

	temp->scope = curHashTable->scope + 1;
	
	if (hashTableList == curHashTable)
		hashTableList = temp;
	curHashTable = temp;
}

//      Go out the scope
void closeScope(void){
	curHashTable = curHashTable->outer;
}

//	Insertion to hash table
int st_insert(char * name, int lineno, int loc, NodeKind kind, int subKind, int arrSize, ExpType type){
	int h = hash(name);
	BucketList l = curHashTable->hashTable[h];

	while ((l != NULL) && (strcmp(name, l->name) != 0))
		l = l->next;

	if (l == NULL){
		if (kind == ExpK)
			return -1;	//Scoping Error
		
		l = (BucketList)malloc(sizeof(struct BucketListRec));
		l->name = name;		//복사해서 줘야함??
		l->loc = loc;
		l->kind = kind;
		l->subKind = subKind;
		l->arrSize = arrSize;
		l->type = type;

		l->lines = (LineList)malloc(sizeof(struct LineListRec));
                l->lines->lineno = lineno;
		l->lines->next = NULL;

		l->next = curHashTable->hashTable[h];
		curHashTable->hashTable[h] = l;
	}
	else{
		LineList t = l->lines;
		while (t->next != NULL) t = t->next;
		t->next = (LineList)malloc(sizeof(struct LineListRec));
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
	
	return 0;
}

int st_lookup(char * name){
	int nextScope = curHashTable->scope;
	HashTableList tableSearched = curHashTable;

	int h = hash(name);
	BucketList l;

	while (nextScope >= 0){
		while (tableSearched->scope != nextScope) tableSearched = tableSearched->outer;
		nextScope--;

		l = tableSearched->hashTable[h];
		while((l != NULL) && (strcmp(name, l->name) != 0))
			l = l->next;
		if ( l )
			return l->loc;
	}

	return -1;
}

void printSymTab(FILE *listing){
	HashTableList cur = hashTableList;
	int i;
	BucketList l;

	while (cur){
		fprintf(listing, "Name          Scope  Loc  V/P/F  Array?  ArrSize  Type  Line Numbers\n");
        	fprintf(listing, "-----------------------------------------------------------------------\n");
		for (i = 0; i < SIZE; i++){
			BucketList l = cur->hashTable[i];
			while ( l ){
				fprintf(listing, "%-14s", l->name);
				fprintf(listing, "%-7d", cur->scope);
				fprintf(listing, "%-5d", l->loc);
				if (l->kind == PrmtK){
					fprintf(listing, "%-7s", "Par");
				}
				else{
					if (l->subKind == FuncK)
						fprintf(listing, "%-7s", "Func");
					else
						fprintf(listing, "%-7s", "Var");
				}
				if (l->subKind == 2){
					fprintf(listing, "%-8s", "Array");	
					fprintf(listing, "%-9d", l->arrSize);
					fprintf(listing, "%-6s", "array");
				}
				else{
					fprintf(listing, "%-8s", "IntArr");
					fprintf(listing, "%-9s", "-");
					if (l->type == Void)
						fprintf(listing, "%-6s", "void");
					else
						fprintf(listing, "%-6s", "int");
				}
				LineList t = l->lines;
				while ( t ){
					fprintf(listing, "%-5d", t->lineno);
					t = t->next;
				}
				fprintf(listing, "\n");
				l = l->next;
			}
		}
		cur = cur->outer;
	}
	
	
	
}
