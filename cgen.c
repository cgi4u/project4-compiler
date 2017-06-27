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

int popStack(){
	IntCont *temp = fpspIntStack;
	int tempi;
	fpspIntStack = fpspIntStack->link;
	tempi = temp->val;
	free(temp);
	return tempi;
}

int fpspInt;

static void cGen(TreeNode *tree);

/* Procedure genDecl generates code at a declaration node */
static void genDcl( TreeNode * tree)
{ 
  	switch (tree->kind.dcl) {
 	case FuncK:
		fprintf(code, "%s:\n", tree->attr.name);
		if (strcmp(tree->attr.name, "main"))
			fpspInt = -8;
		else{
			fpspInt = 0;
			fprintf(code, "\tmove\t$fp, $sp\n");
		}
		cGen(tree->child[2]);
		break;
	/*
  	case VarK:
		if (tree->location < 0){
			fpspInt -= 4;
			tree->location = fpspInt;
			fprintf(code, "\taddiu\t$sp, $sp, -4\n");
		}
		break;
  	case ArrK:
		if (tree->location < 0){
                        fpspInt -= 4 * tree->attr.arrProp.size;
                        tree->location = fpspInt;
			fprintf(code, "\taddiu\t$sp, $sp, %d\n", -4 * tree->attr.arrProp.size);
                }
		break;
	*/
 	default: break;
	}
} 

void ifAdrr(TreeNode *expNode){
	if (expNode->kind.exp == IdK || expNode->kind.exp == ArrIdK){
		//fprintf(code, "\tmove\t$t1, $t0\n");
               	fprintf(code, "\tlw\t$t0, 0($t0)\n");
	}
}

void genStmt( TreeNode * tree ){
	static int selLabelNum = 0;
	int curSelLabelNum;

	static int iterLabelNum = 0;
        int curIterLabelNum;

	TreeNode *temp;
	int beforeInt;

	switch (tree->kind.stmt) {
	case CompK:
		temp = tree->child[0];
		beforeInt = fpspInt;
		while (temp){
			if (temp->kind.dcl == ArrK)
				fpspInt -= 4 * temp->attr.arrProp.size;
			else
				fpspInt -= 4;
			temp->location = fpspInt;
			temp = temp->sibling;
		}
		if (fpspInt != beforeInt)
			fprintf(code, "\taddiu\t$sp, $sp, %d\n", fpspInt - beforeInt);
		cGen(tree->child[1]);
		break;
	case SelK:
		cGen(tree->child[0]);
		/*
		fprintf(code, "\tlw\t$t0, 0($sp)\n");
		fprintf(code, "\taddiu\t$sp, $sp, 4\n");
		fpspInt += 4;
		*/
		ifAdrr(tree->child[0]);
		curSelLabelNum = selLabelNum++;
		fprintf(code, "\tbeqz\t$t0, SL%d\n", curSelLabelNum);
		cGen(tree->child[1]);
		fprintf(code, "SL%d:\n", curSelLabelNum);
		if (tree->child[2])
			cGen(tree->child[2]);
		break;
	case IterK:
		curIterLabelNum = iterLabelNum++;
		fprintf(code, "IL%d:\n", curIterLabelNum);
		cGen(tree->child[0]);
		/*
		fprintf(code, "\tlw\t$t0, 0($sp)\n");
                fprintf(code, "\taddiu\t$sp, $sp, 4\n");
		fpspInt += 4;
		*/
		ifAdrr(tree->child[0]);
		fprintf(code, "\tbeqz\t$t0, IL%d_done\n", curIterLabelNum);
		cGen(tree->child[1]);
		fprintf(code, "\tj\tIL%d\n", curIterLabelNum);
		fprintf(code, "IL%d_done:\n", curIterLabelNum);
	default: break;
	}
}

void genExp(TreeNode *tree){
	static int compLabelNum = 0;
	int curCompLabelNum = 0;

	switch (tree->kind.exp){
	case ConstK:
		fprintf(code, "\tli\t$t0, %d\n", tree->attr.val);
		/*if (expFlag) break;
		fprintf(code, "\taddiu\t$sp, $sp, -4\n");
		fpspInt -= 4;
		fprintf(code, "\tsw\t$t0, 0($sp)\n");*/
		break;
	case IdK:
		if (tree->dclNode == NULL)
			fprintf(code, "\tla\t$t0, %s\n", tree->attr.name);
		else
			fprintf(code, "\tla\t$t0, %d($fp)\n", tree->dclNode->location);
		/*if (expFlag) break;
		fprintf(code, "\taddiu\t$sp, $sp, -4\n");
		fpspInt -= 4;
                fprintf(code, "\tsw\t$t0, 0($sp)\n");*/
		break;
	case ArrIdK:
		cGen(tree->child[0]);
		/*
		fprintf(code, "\tlw\t$t0, 0($sp)\n");
                fprintf(code, "\taddiu\t$sp, $sp, 4\n");
		fpspInt += 4;
		*/
		ifAdrr(tree->child[0]);
		fprintf(code, "\tmove\t$t1, $t0\n");
		fprintf(code, "\tli\t$t2, 4\n");
		fprintf(code, "\tmul\t$t1, $t1, $t2\n");	
		if (tree->dclNode == 0)
                        fprintf(code, "\tla\t$t0, %s\n", tree->attr.arrProp.name);
		else
			fprintf(code, "\tla\t$t0, %d($fp)\n", tree->dclNode->location);
		fprintf(code, "\tadd\t$t0, $t0, $t1\n");
		/*if (expFlag) break;
		fprintf(code, "\taddiu\t$sp, $sp, -4\n");
		fpspInt -= 4;
		fprintf(code, "\tsw\t$t0, 0($sp)\n");*/
		break;
	case OpK:
		cGen(tree->child[0]);
		fprintf(code, "\taddiu\t$sp, $sp, -4\n");
		fpspInt -= 4;
		fprintf(code, "\tsw\t$t0, 0($sp)\n");
		
		cGen(tree->child[1]);

		/*fprintf(code, "\tlw\t$t0, 0($sp)\n");
                fprintf(code, "\taddiu\t$sp, $sp, 4\n");
                fpspInt += 4;*/
                ifAdrr(tree->child[1]);
		fprintf(code, "\tmove\t$t2, $t0\n");

		fprintf(code, "\tlw\t$t0, 0($sp)\n");
                fprintf(code, "\taddiu\t$sp, $sp, 4\n");
                fpspInt += 4;
                ifAdrr(tree->child[0]);
                fprintf(code, "\tmove\t$t1, $t0\n");
		
		switch (tree->attr.op){
		case EQ:
			curCompLabelNum = compLabelNum++;
			fprintf(code, "\tli\t$t0, 1\n");
			fprintf(code, "\tbeq\t$t1, $t2, CL%d\n", curCompLabelNum);
			fprintf(code, "\tli\t$t0, 0\n");
			fprintf(code, "CL%d:\n", curCompLabelNum);
			break;
		case NE:
			curCompLabelNum = compLabelNum++;
                        fprintf(code, "\tli\t$t0, 1\n");
                        fprintf(code, "\tbne\t$t1, $t2, CL%d\n", curCompLabelNum);
                        fprintf(code, "\tli\t$t0, 0\n");
                        fprintf(code, "CL%d:\n", curCompLabelNum);
                        break;
		case LE:
			curCompLabelNum = compLabelNum++;
                        fprintf(code, "\tli\t$t0, 1\n");
                        fprintf(code, "\tble\t$t1, $t2, CL%d\n", curCompLabelNum);
                        fprintf(code, "\tli\t$t0, 0\n");
                        fprintf(code, "CL%d:\n", curCompLabelNum);
                        break;
		case LT:
			curCompLabelNum = compLabelNum++;
                        fprintf(code, "\tli\t$t0, 1\n");
                        fprintf(code, "\tblt\t$t1, $t2, CL%d\n", curCompLabelNum);
                        fprintf(code, "\tli\t$t0, 0\n");
                        fprintf(code, "CL%d:\n", curCompLabelNum);
                        break;
		case GE:
			curCompLabelNum = compLabelNum++;
                        fprintf(code, "\tli\t$t0, 1\n");
                        fprintf(code, "\tbge\t$t1, $t2, CL%d\n", curCompLabelNum);
                        fprintf(code, "\tli\t$t0, 0\n");
                        fprintf(code, "CL%d:\n", curCompLabelNum);
                        break;
		case GT:
			curCompLabelNum = compLabelNum++;
                        fprintf(code, "\tli\t$t0, 1\n");
                        fprintf(code, "\tbgt\t$t1, $t2, CL%d\n", curCompLabelNum);
                        fprintf(code, "\tli\t$t0, 0\n");
                        fprintf(code, "CL%d:\n", curCompLabelNum);
                        break;
		case PLUS:
			fprintf(code, "\tadd\t$t0, $t1, $t2\n");
			break;
		case MINUS:
			fprintf(code, "\tsub\t$t0, $t1, $t2\n");
                        break;
		case TIMES:
			fprintf(code, "\tmul\t$t0, $t1, $t2\n");
                        break;
		case OVER:
			fprintf(code, "\tdiv\t$t0, $t1, $t2\n");
                        break;
		default: break;
		}

		/*
		if (expFlag) break;
		fprintf(code, "\taddiu\t$sp, $sp, -4\n");
                fpspInt -= 4;
                fprintf(code, "\tsw\t$t0, 0($sp)\n");
		*/
		break;
	case AssignK:
		cGen(tree->child[0]);
                fprintf(code, "\taddiu\t$sp, $sp, -4\n");
                fpspInt -= 4;
                fprintf(code, "\tsw\t$t0, 0($sp)\n");		

		cGen(tree->child[1]);

		/*fprintf(code, "\tlw\t$t0, 0($sp)\n");
                fprintf(code, "\taddiu\t$sp, $sp, 4\n");
                fpspInt += 4;*/
                ifAdrr(tree->child[1]);

                fprintf(code, "\tlw\t$t1, 0($sp)\n");
                fprintf(code, "\taddiu\t$sp, $sp, 4\n");
                fpspInt += 4;
                
		fprintf(code, "\tsw\t$t0, 0($t1)\n");
		/*
		if (expFlag) break;
                fprintf(code, "\taddiu\t$sp, $sp, -4\n");
                fpspInt -= 4;
                fprintf(code, "\tsw\t$t1, 0($sp)\n");
		*/
		break;
	case InK:
		fprintf(code, "\tli\t$v0, 4\n");
		fprintf(code, "\tla\t$a0, input\n");
		fprintf(code, "\tsyscall\n");
		
		fprintf(code, "\tli\t$v0, 5\n");
                fprintf(code, "\tsyscall\n");
		fprintf(code, "\tmove\t$t1, $v0\n");

		cGen(tree->child[0]);
		fprintf(code, "\tsw\t$t1, 0($t0)\n");
		break;
	case OutK:
		fprintf(code, "\tli\t$v0, 4\n");
                fprintf(code, "\tla\t$a0, Aoutput\n");
                fprintf(code, "\tsyscall\n");

		cGen(tree->child[0]);
		ifAdrr(tree->child[0]);
		
		fprintf(code, "\tli\t$v0, 1\n");
		fprintf(code, "\tla\t$a0, $t0\n");
		fprintf(code, "\tsyscall\n");
		break;
	default: break;		
	}

}
/*
void genExp(TreeNode *tree, int expFlag){
	switch (tree->kind.exp){
		
	}
}
*/

static void cGen(TreeNode *tree)
{ 
	if (tree != NULL){ 
		switch (tree->nodekind) {
      		case StmtK:
        		genStmt(tree);
        		break;
      		case ExpK:
        		genExp(tree);
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
			fprintf(code, "%s:\t.space %d\n", syntaxTree->attr.name, syntaxTree->location);
		}
		syntaxTree = syntaxTree->sibling;
	}
	fprintf(code, "input:\t.asciiz \"Input Integer:\"\n");
	fprintf(code, "output:\t.asciiz \"Output:\"\n");
	fprintf(code, "\t.text\n");
	fprintf(code, "\t.globl main\n");
}

void codeGen(TreeNode * syntaxTree)
{
	presetMIPS(syntaxTree);	  
	cGen(syntaxTree);
  
	fprintf(code,"\tli $v0, 10\n");
	fprintf(code,"\tsyscall\n");
}
