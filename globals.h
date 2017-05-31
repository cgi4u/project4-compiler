#ifndef _GLOBALS_H_
#define _GLOBALS_H_
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef YYPARSER

#include "cm.tab.h"

#define ENDFILE 0

#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define NO_PARSE FALSE

#define MAXRESERVED 6

typedef int TokenType;

/*
typedef enum
	//book-keeping tokens
	{ENDFILE, ERROR, CMERROR,
	//reserved words
	IF, ELSE, INT, RETURN, VOID, WHILE,
	//multicharacter tokens
	ID, NUM,
	//special symbols
	EQ, NE, LE, LT, GE, GT, PLUS, MINUS, TIMES, OVER, ASSIGN, SEMICOLON, COLON, 
	LPAREN, RPAREN, LSQBRKT, RSQBRKT, LBRACE, RBRACE
	} TokenType;
*/

extern FILE *source;	//source code text file
extern FILE *listing;	//listing output text file(or standard output)

extern int lineno;	//source code line number for listing

extern int Error;

//Syntax tree for parsing

typedef struct arrayProp{
    	char * name;
    	int size;
} arrayProp;

typedef enum {StmtK,ExpK,DclK,PrmtK,TypeK} NodeKind;
typedef enum {CompK,SelK,IterK,RetK} StmtKind;
typedef enum {AssignK,OpK,ConstK,IdK,ArrIdK,CallK} ExpKind;
typedef enum {FuncK,VarK,ArrK} DclKind;
typedef enum {IntK,IntArrK,VoidK} PrmtKind;

//Used for type checking
typedef enum {Void, Integer} ExpType; 

#define MAXCHILDREN 3

typedef struct treeNode{ 
	struct treeNode * child[MAXCHILDREN];
	struct treeNode * sibling;
    	int lineno;
     	NodeKind nodekind;
     	union { StmtKind stmt;
      		ExpKind exp;
          	DclKind dcl;
          	PrmtKind prmt;
     	} kind;
     	union { TokenType op;
		TokenType type;
             	int val;
             	char * name;
             	arrayProp arrProp;
     	} attr;
	ExpType type;
} TreeNode;

#endif
