#include "globals.h"
#include "symtab.h"
#include "cgen.h"

typedef struct intCont{
	int val;
	
	struct intCont *link;
} IntCont;

IntCont *fpspIntStack = NULL;

void pushStack(int val){
	IntCont *temp = fpspIntStack;
	fpspIntStack = (IntCont*)malloc(sizeof(IntCont));
	fpspIntStack->val = val;
	fpspIntStack->link = temp;
}

int popStack(int val){
	iIntCont *temp = fpspIntStack;
	int tempi;
	fpspIntStack = fpspIntStack->link;
	tempi = temp->val;
	free(temp);
	return tempi;
}

int fpspInt = 0;

/* Procedure genDecl generates code at a declaration node */
static void genDcl( TreeNode * tree)
{ 
	TreeNode * p1, * p2;
 	int loadFuncLoc,jmpLoc,funcBodyLoc,nextDeclLoc;
  	int loc;
  	int size;

  	switch (tree->kind.dcl) {
 	case FuncK:
    		//p1 = tree->child[1];
    		p2 = tree->child[2];

    		:isInFunc = TRUE;

    /* generate code to store the location of func. entry */
    loc = -(st_lookup(tree->attr.name));
    loadFuncLoc = emitSkip(1);
    emitRM("ST", ac1, loc, gp, "func: store the location of func. entry");
    /* decrease global offset by 1 */
    --globalOffset;

    /* generate code to unconditionally jump to next declaration */
    jmpLoc = emitSkip(1);
    emitComment(
        "func: unconditional jump to next declaration belongs here");

    /* skip code generation to allow jump to here
       when the function was called */
    funcBodyLoc = emitSkip(0);
    emitComment("func: function body starts here");

    /* backpatch */
    emitBackup(loadFuncLoc);
    emitRM("LDC", ac1, funcBodyLoc, 0, "func: load function location");
    emitRestore();

    /* generate code to store return address */
    emitRM("ST", ac, retFO, mp, "func: store return address");

    /* calculate localOffset and numOfParams */
    localOffset = initFO;
    numOfParams = 0;
    cGen(p1);

    /* generate code for function body */
    if (strcmp(tree->attr.name, "main") == 0)
      mainFuncLoc = funcBodyLoc;

    cGen(p2);

    /* generate code to load pc with return address */
    emitRM("LD", pc, retFO, mp, "func: load pc with return address");

    /* backpatch */
    nextDeclLoc = emitSkip(0);
    emitBackup(jmpLoc);
    emitRM_Abs("LDA", pc, nextDeclLoc,
        "func: unconditional jump to next declaration");
    emitRestore();

    isInFunc = FALSE;

    if (TraceCode) {
      sprintf(buffer, "-> Function (%s)", tree->attr.name);
      emitComment(buffer);
    }

    break;

  case VarK:
  case ArrVarK:
    if (TraceCode) emitComment("-> var. decl.");

    if (tree->kind.decl == ArrVarK)
      size = tree->attr.arr.size;
    else
      size = 1;

    if (isInFunc == TRUE)
      localOffset -= size;
    else
      globalOffset -= size;

    if (TraceCode) emitComment("<- var. decl.");
    break;

  default:
     break;
  }
} /* genDecl */

/* Procedure genParam generates code at a declaration node 
static void genPrmt(TreeNode * tree)
{ 
	switch (tree->kind.stmt) {
 	case ArrParamK:
  	case NonArrParamK:

    --localOffset;
    ++numOfParams;

    if (TraceCode) emitComment("<- param");

    break;

  default:
     break;
  }
}  genParam */

/* Procedure cGen recursively generates code by
 * tree traversal
 */
static void cGen( TreeNode * tree )
{ if (tree != NULL)
  { switch (tree->nodekind) {
      case StmtK:
        genStmt(tree);
        break;
      case ExpK:
        genExp(tree, FALSE);
        break;
      case DclK:
        genDcl(tree);
        break;
      case PrmtK:
        //genPrmt(tree);
        break;
      default:
        break;
    }
    cGen(tree->sibling);
  }
}

/*
void genMainCall() {
  emitRM("LDC", ac, globalOffset, 0, "init: load globalOffset");
  emitRO("ADD", mp, mp, ac, "init: initialize mp with globalOffset");

  if (TraceCode) emitComment("-> Call");

  // generate code to store current mp 
  emitRM("ST", mp, ofpFO, mp, "call: store current mp");
   generate code to push new frame 
  emitRM("LDA", mp, 0, mp, "call: push new frame");
  // generate code to save return in ac
  emitRM("LDA", ac, 1, pc, "call: save return in ac");

  // generate code for unconditional jump to function entry 
  emitRM("LDC", pc, mainFuncLoc, 0, "call: unconditional jump to main() entry");

  // generate code to pop current frame 
  emitRM("LD", mp, ofpFO, mp, "call: pop current frame");

  if (TraceCode) emitComment("<- Call");
}
*/
void presetMIPS(TreeNode * syntaxTree){
	fprintf(code, "\t.data\n");
	while ( syntaxTree ){
		if (syntaxTree->kind.dcl != FuncK){
			fprintf(code, "\t%s:\t.space %d", syntaxTree->attr.name, syntaxTree->location);
		}
		syntaxTree = syntaxTree->sibling;
	}
	fprintf(code, "\t.text\n");
	fprintf(code, "\t.globl main\n");
}

void codeGen(TreeNode * syntaxTree)
{
	presetMIPS(syntaxTree);	  
	cGen(syntaxTree);
  
	emitCode("li $v0, 10");
	emitCode("syscall");
}
