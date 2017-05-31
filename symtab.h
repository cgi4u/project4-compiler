#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "globals.h"

/*	Inserts symbol and its properties to symbol table	*/
void st_insert(char * name, int lineno, int scope, int loc, NodeKind kind, int subKind, int arrSize, TokenType type);

/*	Find the symbol which has the certain name	*/
int st_lookup(char * name);

/*	Prints a formattedx listing of the symbol table contents	*/
void printSymTab(FILE *listing);

#endif
