%{
#define YYPARSER

#include "globals.h"
#include "util.h"
#include "scan.h"

#define YYSTYPE TreeNode *

typedef struct nameNode{
	char * savedName;
	
	struct nameNode * link;
} NameNode;

NameNode * topNameNode = NULL;
static char * savedName;
static int savedNum;
static int savedLineno;
static TreeNode * savedTree;
static int yylex(void);
int yyerror(char * );

void pushNNode(char * name){
	NameNode * temp = (NameNode*)malloc(sizeof(NameNode));
	temp->savedName = name;
	temp->link = topNameNode;
	topNameNode = temp;
}

char * popNNode(void){
	if (!topNameNode) return NULL;
	NameNode * temp = topNameNode;
	topNameNode = topNameNode->link;
	char * retName = temp->savedName;
	free(temp);
	return retName;
}

//int yydebug = 1;

%}

%token IF ELSE INT RETURN VOID WHILE
%token ID NUM
%token EQ NE LE LT GE GT PLUS MINUS TIMES OVER ASSIGN SEMICOLON COLON
%token LPAREN RPAREN LSQBRKT RSQBRKT LBRACE RBRACE
%token ERROR CMERROR

%%

program		: dcl_list
			{ savedTree = $1; }
		;
dcl_list	: dcl_list dcl
		      	{ YYSTYPE t = $1;
			  if (t != NULL){
			  	while (t->sibling != NULL)
					t = t->sibling;
				t->sibling = $2;
				$$ = $1;
			  }
			  else $$ = $2;
			}
		| dcl { $$ = $1; }
		;
dcl		: var_dcl { $$ = $1; }
		| func_dcl { $$ = $1; }
		;
var_dcl		: type_spc ID { savedName = copyString(oldTokenString);} SEMICOLON
			{ $$ = newDclNode(VarK);
			  $$->child[0] = $1;
			  $$->lineno = lineno;
			  $$->attr.name = savedName;
		  	}
		| type_spc ID { savedName = copyString(oldTokenString); } 
		  LSQBRKT NUM { savedNum = atoi(tokenString); } RSQBRKT SEMICOLON
			{ $$ = newDclNode(ArrK);
			  $$->child[0] = $1;
			  $$->lineno = lineno;
			  $$->attr.arrProp.name = savedName;
			  $$->attr.arrProp.size = savedNum;
			}
		;
type_spc	: INT
			{ $$ = newTypeNode();
			  $$->attr.type = INT;
			}
		| VOID
			{ $$ = newTypeNode();
                          $$->attr.type = VOID;
                        }
		;	
func_dcl	: type_spc ID { pushNNode(copyString(oldTokenString)); savedLineno = lineno; } 
		  LPAREN params RPAREN comp_stmt
			{ $$ = newDclNode(FuncK);
			  $$->child[0] = $1;
			  $$->child[1] = $5;
			  $$->child[2] = $7;
			  $$->attr.name = popNNode();
			  $$->lineno = savedLineno;
			}
		;
params		: param_list { $$ = $1; }
		| VOID { $$ = newPrmtNode(VoidK); }
		;
param_list	: param_list COLON param
			{ YYSTYPE t = $1;
                          if (t != NULL){
                                while (t->sibling != NULL)
                                        t = t->sibling;
                                t->sibling = $3;
                                $$ = $1;
                          }
                          else $$ = $3;
                        }
		| param { $$ = $1; }
		;
param		: type_spc ID
			{ $$ = newPrmtNode(IntK);
                          $$->child[0] = $1;
                          $$->attr.name = copyString(oldTokenString);
			  $$->lineno = lineno;
                        }
		| type_spc ID { savedName = copyString(oldTokenString); } LSQBRKT RSQBRKT
			{ $$ = newPrmtNode(IntArrK);
                          $$->child[0] = $1;
                          $$->attr.name = savedName;
			  $$->lineno = lineno;
                        }
		;
comp_stmt	: LBRACE local_dcls stmt_list RBRACE
			{ $$ = newStmtNode(CompK);
			  $$->child[0] = $2;
			  $$->child[1] = $3;
			}
		;
local_dcls	: local_dcls var_dcl
			{ YYSTYPE t = $1;
                          if (t != NULL){
                                while (t->sibling != NULL)
                                        t = t->sibling;
                                t->sibling = $2;
                                $$ = $1;
                          }
                          else $$ = $2;
                        }
		| { $$ = NULL; }
		;
stmt_list	: stmt_list stmt
			{ YYSTYPE t = $1;
                          if (t != NULL){
                                while (t->sibling != NULL)
                                        t = t->sibling;
                                t->sibling = $2;
                                $$ = $1;
                          }
                          else $$ = $2;
                        }
		| { $$ = NULL; }
		;
stmt		: exp_stmt { $$ = $1; }
		| comp_stmt { $$ = $1; }
		| sel_stmt { $$ = $1; }
		| iter_stmt { $$ = $1; }
		| ret_stmt { $$ = $1; }
		;
exp_stmt	: exp SEMICOLON { $$ = $1; }
		| SEMICOLON { $$ = NULL; }
		;
sel_stmt	: IF LPAREN exp RPAREN stmt
		    	{ $$ = newStmtNode(SelK);
			  $$->child[0] = $3;
			  $$->child[1] = $5;
			}
		| IF LPAREN exp RPAREN stmt ELSE stmt
			{ $$ = newStmtNode(SelK); 
                          $$->child[0] = $3;
                          $$->child[1] = $5;
			  $$->child[2] = $7;
                        }
		;
iter_stmt	: WHILE LPAREN exp RPAREN stmt
			{ $$ = newStmtNode(IterK);
			  $$->child[0] = $3;
                          $$->child[1] = $5;
			}
		;
ret_stmt	: RETURN SEMICOLON { $$ = newStmtNode(RetK); }
		| RETURN exp SEMICOLON
			{ $$ = newStmtNode(RetK); 
			  $$->child[0] = $2;
			}
		;
exp		: var ASSIGN exp
			{ $$ = newExpNode(AssignK);
                          $$->child[0] = $1;
                          $$->child[1] = $3;
			  $$->lineno = lineno;
                        }
		| simple_exp { $$ = $1; }
		;
var		: ID 
			{ $$ = newExpNode(IdK);
			  $$->attr.name = copyString(oldTokenString);
			  $$->lineno = lineno;
                        }
		| ID { pushNNode(copyString(oldTokenString)); } 
		  LSQBRKT exp RSQBRKT
			{ $$ = newExpNode(ArrIdK);
                          $$->attr.name = popNNode();
			  $$->child[0] = $4;
			  $$->lineno = lineno;
                        }
		;
simple_exp	: add_exp EQ add_exp
			{ $$ = newExpNode(OpK);
                          $$->child[0] = $1;
			  $$->child[1] = $3;
			  $$->attr.op = EQ;
			  $$->lineno = lineno;
                        }
		| add_exp NE add_exp
                        { $$ = newExpNode(OpK);
                          $$->child[0] = $1;
                          $$->child[1] = $3;
                          $$->attr.op = NE;
			  $$->lineno = lineno;
                        }
		| add_exp LE add_exp
                        { $$ = newExpNode(OpK);
                          $$->child[0] = $1;
                          $$->child[1] = $3;
                          $$->attr.op = LE;
			  $$->lineno = lineno;
                        }
		| add_exp LT add_exp
                        { $$ = newExpNode(OpK);
                          $$->child[0] = $1;
                          $$->child[1] = $3;
                          $$->attr.op = LT;
			  $$->lineno = lineno;
                        }
		| add_exp GE add_exp
                        { $$ = newExpNode(OpK);
                          $$->child[0] = $1;
                          $$->child[1] = $3;
                          $$->attr.op = GE;
			  $$->lineno = lineno;
                        }
		| add_exp GT add_exp
                        { $$ = newExpNode(OpK);
                          $$->child[0] = $1;
                          $$->child[1] = $3;
                          $$->attr.op = GT;
			  $$->lineno = lineno;
                        }
		| add_exp { $$ = $1; }
		;
add_exp		: add_exp PLUS term
			{ $$ = newExpNode(OpK);
                          $$->child[0] = $1;
                          $$->child[1] = $3;
                          $$->attr.op = PLUS;
			  $$->lineno = lineno;
                        }
		| add_exp MINUS term
                        { $$ = newExpNode(OpK);
                          $$->child[0] = $1;
                          $$->child[1] = $3;
                          $$->attr.op = MINUS;
			  $$->lineno = lineno;
                        }
		| term { $$ = $1; }
		;
term		: term TIMES factor
			{ $$ = newExpNode(OpK);
                          $$->child[0] = $1;
                          $$->child[1] = $3;
                          $$->attr.op = TIMES;
			  $$->lineno = lineno;
                        }
		| term OVER factor
			{ $$ = newExpNode(OpK);
                          $$->child[0] = $1;
                          $$->child[1] = $3;
                          $$->attr.op = OVER;
			  $$->lineno = lineno;
                        }
		| factor { $$ = $1; }
		;
factor		: LPAREN exp RPAREN { $$ = $2; }
		| call { $$ = $1; }
		| var { $$ = $1; }
		| NUM 
			{ $$ = newExpNode(ConstK);
                          $$->attr.val = atoi(tokenString);
			  $$->lineno = lineno;
                        }
		;
call		: ID { pushNNode(copyString(oldTokenString)); }
		  LPAREN args RPAREN
			{ $$ = newExpNode(CallK);
                          $$->attr.name = popNNode();
			  $$->child[0] = $4;
			  $$->lineno = lineno;
                        }
		;
args		: arg_list { $$ = $1; }
		| { $$ = NULL; }
		;
arg_list	: arg_list COLON exp
			{ YYSTYPE t = $1;
                          if (t != NULL){
                                while (t->sibling != NULL)
                                        t = t->sibling;
                                t->sibling = $3;
                                $$ = $1;
                          }
                          else $$ = $3;
                        }
		| exp { $$ = $1; }
		;

%%

int yyerror(char * message){ 
	fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
 	fprintf(listing,"Current token: ");
 	printToken(yychar,tokenString);
	Error = TRUE;
 	return 0;
}

static int yylex(void){
	return getToken();
}

TreeNode * parse(void){
	yyparse();
	return savedTree;
}


			  

				
