#include "globals.h"
#include "util.h"
#include "scan.h"
#include "analyze.h"
#include "cgen.h"

/*allocate global variables*/
int lineno = 0;
FILE *source;
FILE *listing;
FILE *code;
int Error = FALSE;

int main(int argc, char *argv[]){
	char pgm[20];	//source code file name
	char *temp;	
	TreeNode * syntaxTree;	

	if (argc != 2){
		fprintf(stderr, "usage: %s <filename>\n", argv[0]);
		exit(1);
	}
	
	strcpy(pgm, argv[1]);
	if (strchr(pgm, '.') == NULL)
		strcat(pgm, ".c");
	source = fopen(pgm, "r");
	if (source == NULL){
		fprintf(stderr, "File %s not found\n", pgm);
                exit(1);
        }
	
	listing = stdout;
	
	if (NO_PARSE){
		fprintf(listing, "   line number\t\t   token\t   lexeme\n");
		fprintf(listing, "--------------------------------------------------------\n");
		while (getToken() != ENDFILE);
	}
	
	syntaxTree = parse();
	if (Error) return -1;
	if (NO_ANALYZE){
		fprintf(listing, "\nSyntax tree:\n");
		printTree(syntaxTree);
	}

	fprintf(listing, "\nBuilding Symbol Table...\n");
	buildSymtab(syntaxTree);
	if (Error) return -1;	
	fprintf(listing, "\nChecking Types...\n");
	typeCheck(syntaxTree);
	if (Error) return -1;
	fprintf(listing, "\nType Checking Finished\n");

	temp = strchr(pgm, '.');
	strcpy(temp, ".asm");
	code = fopen(pgm, "w");
	codeGen(syntaxTree);

	fclose(source);
	fclose(code);
	return 0;
}



