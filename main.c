#include "globals.h"
#include "util.h"
#include "scan.h"

/*allocate global variables*/
int lineno = 0;
FILE *source;
FILE *listing;
int Error = FALSE;

int main(int argc, char *argv[]){
	char pgm[20];	//source code file name
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
	fprintf(listing, "\nSyntax tree:\n");
	printTree(syntaxTree);
	
	fclose(source);
	return 0;
}



