#include "globals.h"
#include "symtab.h"
#include "cgen.h"
/*
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
*/
int fpspInt;

static void cGen(TreeNode *tree);

int voidFuncFlag = 0;
int mainFlag = 0;

/* Procedure genDecl generates code at a declaration node */
static void genDcl( TreeNode * tree)
{ 
  	switch (tree->kind.dcl) {
 	case FuncK:
		fprintf(code, "%s:\n", tree->attr.name);
		if (tree->child[0]->attr.type == VOID)
                        voidFuncFlag = 1;
		if (strcmp(tree->attr.name, "main")){
			fpspInt = -8;
			fprintf(code, "\tsw\t$ra, -4($sp)\n");
                	fprintf(code, "\tsw\t$fp, -8($sp)\n");
        	        fprintf(code, "\tmove\t$fp, $sp\n");
	                fprintf(code, "\taddiu\t$sp, $sp, -8\n");
		}
		else{
			fpspInt = 0;
			fprintf(code, "\tmove\t$fp, $sp\n");
			voidFuncFlag = 2;
			mainFlag = 1;
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

	static int innerVoidFuncFlag = 0;
	
	switch (tree->kind.stmt) {
	case CompK:
		innerVoidFuncFlag = voidFuncFlag;
		voidFuncFlag = 0;
		
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

		if (innerVoidFuncFlag == 1){
			fprintf(code, "\tmove\t$sp, $fp\n");
			fprintf(code, "\tlw\t$ra, -4($fp)\n");
                        fprintf(code, "\tlw\t$fp, -8($fp)\n");
			fprintf(code, "\tjr\t$ra\n");
		}
		else if (innerVoidFuncFlag == 2){
			fprintf(code, "\tli\t$v0, 10\n");
			fprintf(code, "\tsyscall\n");
		}
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
		fprintf(code, "\tbeqz\t$t0, _SL%d\n", curSelLabelNum);
		cGen(tree->child[1]);
		if (tree->child[2]){
			fprintf(code, "\tj\t_SL%d_ELSE\n", curSelLabelNum);
			fprintf(code, "_SL%d:\n", curSelLabelNum);
			cGen(tree->child[2]);
			fprintf(code, "_SL%d_ELSE:\n", curSelLabelNum);
		}
		else{
			fprintf(code, "_SL%d:\n", curSelLabelNum);
		}
		break;
	case IterK:
		curIterLabelNum = iterLabelNum++;
		fprintf(code, "_IL%d:\n", curIterLabelNum);
		cGen(tree->child[0]);
		/*
		fprintf(code, "\tlw\t$t0, 0($sp)\n");
                fprintf(code, "\taddiu\t$sp, $sp, 4\n");
		fpspInt += 4;
		*/
		ifAdrr(tree->child[0]);
		fprintf(code, "\tbeqz\t$t0, _IL%d_done\n", curIterLabelNum);
		cGen(tree->child[1]);
		fprintf(code, "\tj\t_IL%d\n", curIterLabelNum);
		fprintf(code, "_IL%d_done:\n", curIterLabelNum);
		break;
	case RetK:
		if (tree->child[0]){
			cGen(tree->child[0]);
			ifAdrr(tree->child[0]);
			fprintf(code, "\tmove\t$sp, $fp\n");
                        fprintf(code, "\tlw\t$ra, -4($fp)\n");
                        fprintf(code, "\tlw\t$fp, -8($fp)\n");
                        fprintf(code, "\tjr\t$ra\n");
		}
		else{
			if (mainFlag){
				fprintf(code, "\tli\t$v0, 10\n");
                        	fprintf(code, "\tsyscall\n");
			}
			else{
				fprintf(code, "\tmove\t$sp, $fp\n");
                        	fprintf(code, "\tlw\t$ra, -4($fp)\n");
                        	fprintf(code, "\tlw\t$fp, -8($fp)\n");
                        	fprintf(code, "\tjr\t$ra\n");
			}
		}
		break;
	default: break;
	}
}

void genExp(TreeNode *tree){
	static int compLabelNum = 0;
	int curCompLabelNum = 0;
	
	TreeNode *arg_temp_link;
	int arg_temp;

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
		if (tree->dclNode == NULL)
                        fprintf(code, "\tla\t$t0, %s\n", tree->attr.arrProp.name);
		else if (tree->dclNode->nodekind == PrmtK)
			fprintf(code, "\tlw\t$t0, %d($fp)\n", tree->dclNode->location);
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
			fprintf(code, "\tbeq\t$t1, $t2, _CL%d\n", curCompLabelNum);
			fprintf(code, "\tli\t$t0, 0\n");
			fprintf(code, "_CL%d:\n", curCompLabelNum);
			break;
		case NE:
			curCompLabelNum = compLabelNum++;
                        fprintf(code, "\tli\t$t0, 1\n");
                        fprintf(code, "\tbne\t$t1, $t2, _CL%d\n", curCompLabelNum);
                        fprintf(code, "\tli\t$t0, 0\n");
                        fprintf(code, "_CL%d:\n", curCompLabelNum);
                        break;
		case LE:
			curCompLabelNum = compLabelNum++;
                        fprintf(code, "\tli\t$t0, 1\n");
                        fprintf(code, "\tble\t$t1, $t2, _CL%d\n", curCompLabelNum);
                        fprintf(code, "\tli\t$t0, 0\n");
                        fprintf(code, "_CL%d:\n", curCompLabelNum);
                        break;
		case LT:
			curCompLabelNum = compLabelNum++;
                        fprintf(code, "\tli\t$t0, 1\n");
                        fprintf(code, "\tblt\t$t1, $t2, _CL%d\n", curCompLabelNum);
                        fprintf(code, "\tli\t$t0, 0\n");
                        fprintf(code, "_CL%d:\n", curCompLabelNum);
                        break;
		case GE:
			curCompLabelNum = compLabelNum++;
                        fprintf(code, "\tli\t$t0, 1\n");
                        fprintf(code, "\tbge\t$t1, $t2, _CL%d\n", curCompLabelNum);
                        fprintf(code, "\tli\t$t0, 0\n");
                        fprintf(code, "_CL%d:\n", curCompLabelNum);
                        break;
		case GT:
			curCompLabelNum = compLabelNum++;
                        fprintf(code, "\tli\t$t0, 1\n");
                        fprintf(code, "\tbgt\t$t1, $t2, _CL%d\n", curCompLabelNum);
                        fprintf(code, "\tli\t$t0, 0\n");
                        fprintf(code, "_CL%d:\n", curCompLabelNum);
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
		fprintf(code, "\tla\t$a0, _input\n");
		fprintf(code, "\tsyscall\n");
		
		fprintf(code, "\tli\t$v0, 5\n");
                fprintf(code, "\tsyscall\n");
		fprintf(code, "\tmove\t$t0, $v0\n");
		break;
	case OutK:
		fprintf(code, "\tli\t$v0, 4\n");
                fprintf(code, "\tla\t$a0, _output\n");
                fprintf(code, "\tsyscall\n");

		cGen(tree->child[0]);
		ifAdrr(tree->child[0]);
		
		fprintf(code, "\tli\t$v0, 1\n");
		fprintf(code, "\tmove\t$a0, $t0\n");
		fprintf(code, "\tsyscall\n");

		fprintf(code, "\tli\t$v0, 4\n");
                fprintf(code, "\tla\t$a0, _newline\n");
                fprintf(code, "\tsyscall\n");
		break;
	case CallK:
		arg_temp_link = tree->child[0];
	        arg_temp = 0;

		while (arg_temp_link){
			arg_temp++;
			arg_temp_link = arg_temp_link->sibling;
		}
		if (arg_temp){
			fprintf(code, "\taddiu\t$sp, $sp, %d\n", -4 * arg_temp);
		}

		arg_temp_link = tree->child[0];
		arg_temp = 0;

		while (arg_temp_link){
			cGen(arg_temp_link);
			if (arg_temp_link->type != Array)
				ifAdrr(arg_temp_link);
			else
				fprintf(code, "\tadd\t$t0, $fp, t0\n");

                  	fprintf(code, "\tsw\t$t0, %d($sp)\n", arg_temp);
			arg_temp += 4;
                        arg_temp_link = arg_temp_link->sibling;
                }

		//fprintf(code, "\tsw\t$ra, -4($sp)\n");
		//fprintf(code, "\tsw\t$fp, -8($sp)\n");
		//fprintf(code, "\tmove\t$fp, $sp\n");
		//fprintf(code, "\taddiu\t$sp, $sp, -8\n");
		fprintf(code, "\tjal\t%s\n", tree->attr.name);
		fprintf(code, "\taddiu\t$sp, $sp, %d\n", arg_temp);
		
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
	fprintf(code, "_input:\t.asciiz \"Input : \"\n");
	fprintf(code, "_output:\t.asciiz \"Output : \"\n");
	fprintf(code, "_newline:\t.asciiz \"\\n\"\n");
	fprintf(code, "\t.text\n");
	fprintf(code, "\t.globl main\n");
}

void codeGen(TreeNode * syntaxTree)
{
	presetMIPS(syntaxTree);	  
	cGen(syntaxTree);
}
