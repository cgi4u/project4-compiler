#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

#define SIZE 211

//      The list of line numbers for a symbol
typedef struct LineListRec{
        int lineno;

        struct LineListRec *next;
} * LineList;

//      Bucket list for each symbol. Containing its properties
typedef struct BucketListRec{
        char *name;
        LineList lines;
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
	int location;

        struct HashTableRec *outer;
        struct HashTableRec *inner;
} * HashTableList;

/*	Initialze symbol table list	*/
void initSymbolTable(void);

void intoNewScope(void);
void closeScope(void);

/*	Inserts symbol and its properties to symbol table	*/
void st_insert(char * name, int lineno, NodeKind kind, int subKind, int arrSize, TokenType type, BucketList found);

void insertAllPrmt(BucketList);

/*	Find the symbol which has the certain name	*/
BucketList st_lookup(char * name, int lookupMode);

/*	Prints a formattedx listing of the symbol table contents	*/
void printSymTab(FILE *listing);

#endif
