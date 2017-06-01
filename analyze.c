#include "globals.h"
#include "symtab.h"
#include "analyze.h"

//static int location = 0;
//static int locBefore;

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

static void insertPrmtQueue(char *name, int lineno, PrmtKind pkind){
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

	prmtQueueEnd->lines = (LineList)malloc(sizeof(struct LineListRec));
        prmtQueueEnd->lines->lineno = lineno;
        prmtQueueEnd->lines->next = NULL;
}

static int insertNode(TreeNode *t){
	BucketList found;
		BucketList searchedPar = prmtQueueStart; //Only used for parameter case

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
				st_insert(t->attr.name, t->lineno, DclK, FuncK, 0, t->child[0]->attr.type, NULL);
				break;
			case VarK:
				found = st_lookup(t->attr.name, 1);
                                if ( found ){
                                        fprintf(listing, "ERROR in line %d : declaration of a duplicated\n", t->lineno);
                                        fprintf(listing, "\t\t\tfirst declared at line %d\n", found->lines->lineno);
                                        return -1;
                                }
                                st_insert(t->attr.name, t->lineno, DclK, VarK, 0, t->child[0]->attr.type, NULL);
                                break;
			case ArrK:
				found = st_lookup(t->attr.arrProp.name, 1);
                                if ( found ){
                                        fprintf(listing, "ERROR in line %d : declaration of a duplicated\n", t->lineno);
                                        fprintf(listing, "\t\t\tfirst declared at line %d\n", found->lines->lineno);
                                        return -1;
                                }
                                st_insert(t->attr.arrProp.name, t->lineno, DclK, ArrK, t->attr.arrProp.size, t->child[0]->attr.type, NULL);
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
                                insertPrmtQueue(t->attr.name, t->lineno, t->kind.prmt);
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
                                st_insert(NULL, t->lineno, 0, 0, 0, 0, found);
				break;
			case CallK:
                                found = st_lookup(t->attr.name, 0);
                                if ( !found ){
                                        fprintf(listing, "ERROR in line %d : function %s not exist\n", t->lineno, t->attr.name);
                                        return -1;
                                }
                                st_insert(NULL, t->lineno, 0, 0, 0, 0, found);
			default: break;
		}	
	default: break;
	}
	
	return 0;
}

void buildSymtabNode(TreeNode *t){
	int compFlag = 0;

        if (t != NULL){
                if ((t->nodekind == StmtK) && (t->kind.stmt == CompK)){
                        compFlag = 1;
                        intoNewScope();

			if (prmtQueueStart){
                                insertAllPrmt(prmtQueueStart);
                                prmtQueueEnd = prmtQueueStart = NULL;
                        }
                }

                if (insertNode(t) == -1){
			Error = -1;
			return;
		}
                {
                        int i;
                        for (i = 0; i < MAXCHILDREN; i++)
                                buildSymtabNode(t->child[i]);
                }
                buildSymtabNode(t->sibling);

                if (compFlag){
                        closeScope();
                }
        }
}

void buildSymtab(TreeNode *syntaxTree){
	initSymbolTable();
	buildSymtabNode(syntaxTree);
	fprintf(listing, "\nSymbol table:\n");
	printSymTab(listing);
}
