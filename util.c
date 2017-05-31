#include "globals.h"
#include "util.h"
#include "scan.h"

void printToken(TokenType token, const char *tokenString){
	switch(token){
		case IF:	fprintf(listing, "IF"); break;
		case ELSE:	fprintf(listing, "ELSE"); break;
		case INT:	fprintf(listing, "INT"); break;
		case VOID:	fprintf(listing, "VOID"); break;
		case WHILE:	fprintf(listing, "WHILE"); break;
		case RETURN:	fprintf(listing, "RETURN"); break;
		case EQ:	fprintf(listing, "=="); break;
		case NE:	fprintf(listing, "!="); break;
		case LE:	fprintf(listing, "<="); break;
		case LT:	fprintf(listing, "<"); break;
		case GE:	fprintf(listing, ">="); break;
		case GT:	fprintf(listing, ">"); break;
		case PLUS:	fprintf(listing, "+"); break;
                case MINUS:	fprintf(listing, "-"); break;
                case TIMES:	fprintf(listing, "*"); break;
                case OVER:	fprintf(listing, "/"); break;
                case ASSIGN:	fprintf(listing, "="); break;
                case SEMICOLON:	fprintf(listing, ";"); break;
		case COLON:	fprintf(listing, ","); break;
		case LPAREN:	fprintf(listing, "("); break;
                case RPAREN:	fprintf(listing, ")"); break;
                case LSQBRKT:	fprintf(listing, "["); break;
                case RSQBRKT:	fprintf(listing, "]"); break;
                case LBRACE:	fprintf(listing, "{"); break;
                case RBRACE:	fprintf(listing, "}"); break;
		case ID:	fprintf(listing, "ID"); break;
		case NUM:	fprintf(listing, "NUM"); break;
		case ERROR:	
		case CMERROR:	fprintf(listing, "ERROR"); break;
		case ENDFILE:	fprintf(listing, "EOF");
	}
	
	if (token == CMERROR)
		fprintf(listing, "\t\tComment Error\n");
	else
		fprintf(listing, "\t\t%s\n", tokenString);
}

TreeNode * newStmtNode(StmtKind kind){
	TreeNode *t = (TreeNode*)malloc(sizeof(TreeNode));
	int i;
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else{
		for (i = 0; i < MAXCHILDREN; i++)
			t->child[i] = NULL;
		t->sibling = NULL;
		t->nodekind = StmtK;
		t->kind.stmt = kind;
		t->lineno = lineno;
	}

	return t;
}


TreeNode * newExpNode(ExpKind kind){
        TreeNode *t = (TreeNode*)malloc(sizeof(TreeNode));
        int i;
        if (t == NULL)
                fprintf(listing, "Out of memory error at line %d\n", lineno);
        else{
                for (i = 0; i < MAXCHILDREN; i++)
                        t->child[i] = NULL;
                t->sibling = NULL;
                t->nodekind = ExpK;
                t->kind.exp = kind;
                t->lineno = lineno;
        }

        return t;
}

TreeNode * newDclNode(DclKind kind){
        TreeNode *t = (TreeNode*)malloc(sizeof(TreeNode));
        int i;
        if (t == NULL)
                fprintf(listing, "Out of memory error at line %d\n", lineno);
        else{
                for (i = 0; i < MAXCHILDREN; i++)
                        t->child[i] = NULL;
                t->sibling = NULL;
                t->nodekind = DclK;
                t->kind.dcl = kind;
                t->lineno = lineno;
        }

        return t;
}

TreeNode * newPrmtNode(PrmtKind kind){
        TreeNode *t = (TreeNode*)malloc(sizeof(TreeNode));
        int i;
        if (t == NULL)
                fprintf(listing, "Out of memory error at line %d\n", lineno);
        else{
                for (i = 0; i < MAXCHILDREN; i++)
                        t->child[i] = NULL;
                t->sibling = NULL;
                t->nodekind = PrmtK;
                t->kind.prmt = kind;
                t->lineno = lineno;
        }

        return t;
}

TreeNode * newTypeNode(){
        TreeNode *t = (TreeNode*)malloc(sizeof(TreeNode));
        int i;
        if (t == NULL)
                fprintf(listing, "Out of memory error at line %d\n", lineno);
        else{
                for (i = 0; i < MAXCHILDREN; i++)
                        t->child[i] = NULL;
                t->sibling = NULL;
                t->nodekind = TypeK;
                t->lineno = lineno;
        }

        return t;
}


char * copyString(char * s){
        int n;
        char * t;
        if (s == NULL) return NULL;
	n = strlen(s)+1;
	t = (char*)malloc(n*sizeof(char));
	if (t == NULL)
		fprintf(listing, "Out of memory error at line %d\n", lineno);
	else strcpy(t, s);
	return t;
}

static int indentno = 0;

#define INDENT indentno += 2
#define UNINDENT indentno -= 2

static void printSpaces(void){
	int i;
	for (i = 0; i < indentno; i++)
		fprintf(listing, " ");
}

void printTree( TreeNode * tree){
	int i;

	static int level = 0;
	int elseFlag = FALSE;
	int compStmtFlag = FALSE;
 	

	INDENT;
	while (tree != NULL){
		printSpaces();
		if (tree->nodekind == StmtK){
			switch(tree->kind.stmt){
				case CompK:
					fprintf(listing, "Compound statment\n");
					level++;
					compStmtFlag = TRUE;
					break;
				case SelK:
					fprintf(listing, "If(the first expression is condition)\n");
					if (tree->child[2] != NULL) elseFlag = TRUE;
					break;
				case IterK:
					fprintf(listing, "While\n");
					break;
				case RetK:
					fprintf(listing, "Return\n");
					break;	
				default:
					fprintf(listing, "Unknown Node Type\n");
			}
		}
		else if (tree->nodekind == ExpK){
			switch(tree->kind.exp){
				case AssignK:
					fprintf(listing, "Assign(first one variable is the left side)\n");
					break;
				case OpK:
					fprintf(listing, "Op: ");
					printToken(tree->attr.op, "\0");
					break;
				case ConstK:
					fprintf(listing, "Const: %d\n", tree->attr.val);
					break;
				case IdK:
					fprintf(listing, "ID: %s\n", tree->attr.name);
					break;
				case ArrIdK:
					fprintf(listing, "Array ID(expression below is index): %s\n", tree->attr.name);
					break;
				case CallK:
					fprintf(listing, "Function Call(expressions below are arguments): %s\n", tree->attr.name);
					break;
				default:
					fprintf(listing, "Unknown Node Type\n");
			}
		}
		else if (tree->nodekind == DclK){
			switch(tree->kind.dcl){
				case FuncK:
					fprintf(listing, "Function Declaration\n");
					INDENT;
                                        printSpaces();
                                        fprintf(listing, "ID: %s\n", tree->attr.name);
                                        UNINDENT;
					break;
				case VarK:
					fprintf(listing, "Variable Declaration");
					if (level != 0)
						fprintf(listing, "(Local)\n");
					else
						fprintf(listing, "(Global)\n");
					INDENT;
					printSpaces();
					fprintf(listing, "ID: %s\n", tree->attr.name);
					UNINDENT;
					break;
				case ArrK:
					fprintf(listing, "Variable Declaration(Array)");
					if (level != 0)
                                                fprintf(listing, "(Local)\n");
                                        else
                                                fprintf(listing, "(Global)\n");
					INDENT;
                                        printSpaces();
                                        fprintf(listing, "ID: %s\n", tree->attr.arrProp.name);
					printSpaces();
					fprintf(listing, "SIZE: %d\n", tree->attr.arrProp.size);
                                        UNINDENT;
                                        break;
				default:
                                        fprintf(listing, "Unknown Node Type\n");
			}
		}
		else if (tree->nodekind == PrmtK){
			switch(tree->kind.prmt){
				case IntK:
					fprintf(listing, "Parameter\n");
					INDENT;
                                        printSpaces();
                                        fprintf(listing, "ID: %s\n", tree->attr.name);
                                        UNINDENT;
					break;
				case IntArrK:
					fprintf(listing, "Parameter(array)\n");
					INDENT;
                                        printSpaces();
                                        fprintf(listing, "ID: %s\n", tree->attr.name);
                                        UNINDENT;
					break;
				case VoidK:
					fprintf(listing, "No Parameter\n");
                                        break;
				default:
                                        fprintf(listing, "Unknown Node Type\n");
			}
		}
		else if (tree->nodekind == TypeK){
			fprintf(listing,"Type: ");
			if (tree->attr.type == INT)
				fprintf(listing,"int\n");
			else
				fprintf(listing,"void\n");
		}
		else fprintf(listing, "Unknown Node Type\n");


		for (i = 0; i < MAXCHILDREN; i++){
			if (elseFlag && i == 2){
				printSpaces();
				fprintf(listing, "Else\n");
			}
			if (tree->child[i]) printTree(tree->child[i]);
		}

		if (compStmtFlag) level--;
		tree = tree->sibling;
	}
	UNINDENT;
}
