#include "globals.h"
#include "symtab.h"
#include "analyze.h"

static BucketList prmtQueueStart = NULL;
static BucketList prmtQueueEnd = NULL;

/*
static void traverse(TreeNode *t, int (*preProc)(TreeNode*), int (*postProc)(TreeNode*)){
	int compFlag = 0;	

	if (t != NULL){
		if ((t->nodekind == StmtK) && (t->kind.stmt == CompK)){
			compFlag = 1;
			locBefore = location;
			location = 0;
			intoNewScope();
		}

		preProc(t);
		{
			int i;
			for (i = 0; i < MAXCHILDREN; i++)
				traverse(t->child[i], preProc, postProc);
		}
		postProc(t);
		traverse(t->sibling, preProc, postProc);

		if (compFlag){
			closeScope();
			location = locBefore;
		}
	}
}

static int nullProc(TreeNode *t){
	return 0;
}
*/

static void insertPrmtQueue(char *name, int lineno, PrmtKind pkind, TreeNode *symbolNode){
	if (prmtQueueEnd){
		prmtQueueEnd->next = (BucketList)malloc(sizeof(struct BucketListRec));
		prmtQueueEnd = prmtQueueEnd->next;	
	}
	else{
		prmtQueueEnd = prmtQueueStart = (BucketList)malloc(sizeof(struct BucketListRec));
	}
	
	prmtQueueEnd->name = name;
        prmtQueueEnd->loc = 0;
        prmtQueueEnd->kind = PrmtK;
        prmtQueueEnd->subKind = pkind;
        prmtQueueEnd->arrSize = 0;
        prmtQueueEnd->type = INT;
	prmtQueueEnd->next = NULL;
	prmtQueueEnd->symbolNode = symbolNode;

	prmtQueueEnd->lines = (LineList)malloc(sizeof(struct LineListRec));
        prmtQueueEnd->lines->lineno = lineno;
        prmtQueueEnd->lines->next = NULL;
}
	
static int returnFlag = 0;

static int insertNode(TreeNode *t){
	BucketList found;
	BucketList searchedPar = prmtQueueStart; //Only used for parameter case
	
	static int mainFlag = 0;

	switch (t->nodekind){
	case DclK:
		switch (t->kind.dcl){
			case FuncK:
				found = st_lookup(t->attr.name, 1);
				if ( found ){
					fprintf(listing, "ERROR in line %d : declaration of a duplicated\n", t->lineno);
					fprintf(listing, "\t\t\tfirst declared at line %d\n", found->lines->lineno);
					return -1;
				}

				if (mainFlag){
					BucketList mainBucket = st_lookup("main", 0);
                                        fprintf(listing, "ERROR in line %d : main should be the last function\n", mainBucket->lines->lineno);
                                        return -1;
                                }
					
				if (strcmp(t->attr.name, "main") == 0)
					mainFlag = 1;

				st_insert(t->attr.name, t->lineno, DclK, FuncK, 0, t->child[0]->attr.type, NULL, t);
				break;
			case VarK: 
				found = st_lookup(t->attr.name, 1);
                                if ( found ){
                                        fprintf(listing, "ERROR in line %d : declaration of a duplicated\n", t->lineno);
                                        fprintf(listing, "\t\t\tfirst declared at line %d\n", found->lines->lineno);
                                        return -1;
                                }
                                st_insert(t->attr.name, t->lineno, DclK, VarK, 0, t->child[0]->attr.type, NULL, t);
				break;
			case ArrK:
				found = st_lookup(t->attr.arrProp.name, 1);
                                if ( found ){
                                        fprintf(listing, "ERROR in line %d : declaration of a duplicated\n", t->lineno);
                                        fprintf(listing, "\t\t\tfirst declared at line %d\n", found->lines->lineno);
                                        return -1;
                                }
                                st_insert(t->attr.arrProp.name, t->lineno, DclK, ArrK, t->attr.arrProp.size, t->child[0]->attr.type, NULL, t);
				break;
		}
		break;
        case PrmtK:
		switch (t->kind.prmt){
                        case IntK:
			case IntArrK:
				while ((searchedPar != NULL) && (strcmp(searchedPar->name, t->attr.name) != 0)){
					searchedPar = searchedPar->next;
				}
				if ( searchedPar != NULL ){
                                        fprintf(listing, "ERROR in line %d : declaration of a duplicated\n", t->lineno);
                                        fprintf(listing, "\t\t\tfirst declared at line %d\n", searchedPar->lines->lineno);
                                        return -1;
                                }
                                insertPrmtQueue(t->attr.name, t->lineno, t->kind.prmt, t);
			default: break;
                }
		break;
	case ExpK:
		switch (t->kind.exp){
			case IdK:
			case ArrIdK:
				found = st_lookup(t->attr.name, 0);
                                if ( !found ){
                                        fprintf(listing, "ERROR in line %d : variable %s not exist\n", t->lineno, t->attr.name);
                                        return -1;
                                }
                                st_insert(NULL, t->lineno, 0, 0, 0, 0, found, NULL);
				t->location = found->symbol_location;
				if (t->location < 0)
					t->dclNode = found->symbolNode;
				else
					t->dclNode = NULL;
				break;
			case CallK:
                                found = st_lookup(t->attr.name, 0);
                                if ( !found ){
                                        fprintf(listing, "ERROR in line %d : function %s not exist\n", t->lineno, t->attr.name);
                                        return -1;
                                }
                                st_insert(NULL, t->lineno, 0, 0, 0, 0, found, NULL);
			default: break;
		}
	case StmtK:
                if (t->kind.stmt == RetK)
                        returnFlag = 1;
                break;
	default: break;
	}
	
	return 0;
}

int buildSymtabNode(TreeNode *t){
	int compFlag = 0;
	static int funcFlag = 0;
        int innerFuncFlag = 0;

        if (t != NULL){
		if ((t->nodekind == DclK) && (t->kind.stmt == FuncK)
                                        && (t->child[0]->attr.type == INT)){
                        funcFlag = 1;
                }
                if ((t->nodekind == StmtK) && (t->kind.stmt == CompK)){
                        compFlag = 1;
                        intoNewScope();

			if (prmtQueueStart){
                                insertAllPrmt(prmtQueueStart);
                                prmtQueueEnd = prmtQueueStart = NULL;
                        }
			
			if (funcFlag == 1){
                                funcFlag = 0;
                                innerFuncFlag = 1;
                                returnFlag = 0;
                        }
                }

                if (insertNode(t) == -1){
			Error = TRUE;
			return -1;
		}
                {
                        int i;
                        for (i = 0; i < MAXCHILDREN; i++){
                                if (buildSymtabNode(t->child[i]) == -1)
                        		return -1;
			}
                }
                if (buildSymtabNode(t->sibling) == -1)
			return -1;

                if (compFlag){
                        closeScope();
			if (innerFuncFlag == 1 && returnFlag == 0){
                                fprintf(listing, "ERROR in line %d : return statement not exist\n", t->lineno);
                                Error = TRUE;
                                return -1;
                        }
                }
        }

	return 0;
}

void buildSymtab(TreeNode *syntaxTree){
	initSymbolTable();
	if (buildSymtabNode(syntaxTree) == -1)
		return;
	fprintf(listing, "\nSymbol table:\n");
	printSymTab(listing);
}

static TreeNode *root;
static ExpType curFuncRetType;

int checkNode(TreeNode *t){
	BucketList found;	

	switch (t->nodekind){
	case StmtK:
		switch (t->kind.stmt){
		case SelK:  
		case IterK: 
			if (t->child[0]->type != Integer){
				fprintf(listing, "ERROR in line %d : condition must be integer type\n", t->child[0]->lineno);               
                                return -1;	
			}
			break;
		case RetK: 
			if (t->child[0]){
				if (curFuncRetType != t->child[0]->type){
                                	fprintf(listing, "ERROR in line %d : wrong return type\n", t->child[0]->lineno);
                                	return -1;
                        	}
			}
			else{
				if (curFuncRetType != Void){                                        
					fprintf(listing, "ERROR in line %d : there should be a return value\n", t->lineno);
                                        return -1;
                                }
			}
		default: break;
		}
		break;
	case ExpK:
		switch (t->kind.exp){
		case AssignK:
		case OpK:
			if ((t->child[0]->type != Integer) || (t->child[1]->type != Integer)){
				fprintf(listing, "ERROR in line %d : invalid operand\n", t->lineno);                   
                                return -1;
			} 
			t->type = Integer;
			break;
		case ConstK:
			t->type = Integer;
                        break;
		case IdK:
			found = st_lookup(t->attr.name, 0);
			if (found->subKind == 2)
				t->type = Array;
			else
				t->type = Integer;
                        break;
		case ArrIdK:	
			found = st_lookup(t->attr.name, 0);
			if (found->subKind != 2){
				fprintf(listing, "ERROR in line %d : array name is required in array expression\n", t->lineno);
                                return -1;
			}
			if (t->child[0]->type != Integer){
				fprintf(listing, "ERROR in line %d : array index must be an integer\n", t->lineno);
                                return -1;
			}
			t->type = Integer;
			break;
		case CallK:
			found = st_lookup(t->attr.name, 0);
			if ((found->kind != DclK) || (found->subKind != FuncK)){
                                fprintf(listing, "ERROR in line %d : function name is required in function call\n", t->lineno);
                                return -1;
                        }

			{
				TreeNode *par = root, *arg = t->child[0];
				while (strcmp(par->attr.name, t->attr.name) != 0){
					par = par->sibling;
				}
				par = par->child[1];
					
				if (!((par->kind.prmt == VoidK) && !arg)){
					while ( par || arg ){
						if (!(par && arg && (par->type == arg->type))){
                                			fprintf(listing, "ERROR in line %d : argument type error\n", t->lineno);
                                			return -1;
                        			}
						par = par->sibling;
						arg = arg->sibling;
					}
				}
			}

			if (found->type == INT)
				t->type = Integer;
			else
				t->type = Void;
			
			break;
		case InK:
		case OutK:
			if (t->child[0]->type != Integer){
                                fprintf(listing, "ERROR in line %d : cannot use void expression for output\n", t->lineno);
                                return -1;
                        }

			t->type = Void;
			break;
		default: break;
		}
		break;
	case DclK:
		switch (t->kind.dcl){
		case VarK: 
		case ArrK:
			if (t->child[0]->attr.type == VOID){
				fprintf(listing, "ERROR in line %d : variable cannot have void type\n", t->lineno);	
				return -1;
			}
			break;
		case FuncK:
			if (strcmp(t->attr.name, "main") == 0){
				if (t->child[0]->attr.type != VOID){
					fprintf(listing, "ERROR in line %d : main fuction should return void\n", t->lineno); 
                                        return -1;
				}
				if (t->child[1]->kind.prmt != VoidK){
					fprintf(listing, "ERROR in line %d : main must not have a parameter\n", t->lineno);
                                        return -1;
				}
			}
		default: break;
		}
		break;
	case PrmtK:
		switch (t->kind.prmt){
		case IntK:
			if (t->child[0]->attr.type == VOID){
				fprintf(listing, "ERROR in line %d : parameter cannot have void type\n", t->lineno);
				return -1;
			}
			t->type = Integer;
			break;
		case IntArrK:
			if (t->child[0]->attr.type == VOID){                                
				fprintf(listing, "ERROR in line %d : parameter cannot have void type\n", t->lineno);
                                return -1;
                        }
			t->type = Array;
			break;
		default: break;
		}
	default: break;
	}
	
	return 0;
}

int checkTree(TreeNode *t){
	int compFlag = 0;

        if (t != NULL){
		if ((t->nodekind == StmtK) && (t->kind.stmt == CompK)){
			compFlag = 1;
                        inScope();
                }

		if ((t->nodekind == DclK) && (t->kind.dcl == FuncK)){ 
			if (t->child[0]->attr.type == VOID)
                                curFuncRetType = Void;
                        else
                                curFuncRetType = Integer;
		}

                {
                        int i;
                        for (i = 0; i < MAXCHILDREN; i++){
                                if (checkTree(t->child[i]) == -1)
                                        return -1;
                        }
                }
		if (checkNode(t) == -1){
                        Error = TRUE;
                        return -1;
                }
                if (checkTree(t->sibling) == -1)
                        return -1;

		if (compFlag){
                        outScope();
                }
        }
	return 0;
}

void typeCheck(TreeNode *syntaxTree){
	root = syntaxTree;
	checkTree(syntaxTree);
}
