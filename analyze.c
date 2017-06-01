#include "globals.h"
#include "symtab.h"
#include "analyze.h"

static int location = 0;
static int locBefore;

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

static int insertNode(TreeNode *t){
	switch (t->nodekind){
	case DclK:
		switch (t->kind.dcl){
			case FuncK:
				st_insert(t->attr.name, t->lineno, location++, DclK, FuncK, 0, t->child[0]->attr.type, NULL);
				break;
			case VarK:
			case ArrK:
				break;
		}
        case PrmtK:
		switch (t->kind.prmt){
                        case IntK:
                        case VoidK:
                        case IntArrK:
				break;
                }
	case ExpK:
		switch (t->kind.exp){
			case IdK:
			case ArrIdK:
			default: break;
		}
		break;
	default: break;
	}
	
	return 0;
}
