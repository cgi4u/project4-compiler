%{
#include "globals.h"
#include "util.h"
#include "scan.h"

char tokenString[MAXTOKENLEN+1];
char oldTokenString[MAXTOKENLEN+1];
%}

digit		[0-9]
number		{digit}+
letter		[a-zA-Z]
identifier	{letter}+
newline		\n
whitespace	[ \t]+

%%

"if"		{return IF;}
"else"		{return ELSE;}
"int"		{return INT;}
"return"	{return RETURN;}
"void"		{return VOID;}
"while"		{return WHILE;}
"=="		{return EQ;}
"!="		{return NE;}
"<="		{return LE;}
"<"		{return LT;}
">="		{return GE;}
">"		{return GT;}
"/*"		{
		
		register int c;
		int start_lineno = lineno;
		
		while ( 1 ){
			while ((c = input()) != '*' && c != EOF){
				if (c == '\n')
					lineno++;
			}
			
			if (c == '*'){
				c = input();
				if (c == '/')
					break;
				unput(c);
			}

			if (c == EOF){
				fprintf(listing, "\t%d\t\tERROR\t\t/*\n", start_lineno);
				return CMERROR;
			}
		}
		
		}
"+"		{return PLUS;}
"-"		{return MINUS;}
"*"		{return TIMES;}
"/"		{return OVER;}
"="		{return ASSIGN;}
";"		{return SEMICOLON;}
","		{return COLON;}
"("		{return LPAREN;}
")"		{return RPAREN;}
"["		{return LSQBRKT;}
"]"		{return RSQBRKT;}
"{"		{return LBRACE;}
"}"		{return RBRACE;}
{number}        {return NUM;}
{identifier}	{return ID;}
{newline}	{lineno++;}
{whitespace}	{/*skip*/}
.		return ERROR;

%%

TokenType getToken(void){
	static int firstTime = TRUE;
	TokenType currentToken;

	if (firstTime){
		firstTime = FALSE;
		lineno++;
		yyin = source;
		yyout = listing;
	}
	else strncpy(oldTokenString, tokenString, MAXTOKENLEN);

	currentToken = yylex();
	strncpy(tokenString, yytext, MAXTOKENLEN);

	if (NO_PARSE){
		fprintf(listing, "\t%d\t\t", lineno);
		printToken(currentToken, tokenString);
	}
	
	return currentToken;
}
