#ifndef _UTIL_H_
#define _UTIL_H_

void printToken(TokenType, const char*);

TreeNode * newStmtNode(StmtKind);
TreeNode * newExpNode(ExpKind);
TreeNode * newDclNode(DclKind);
TreeNode * newPrmtNode(PrmtKind);
TreeNode * newTypeNode();

char * copyString(char*);

void printTree( TreeNode * );

#endif
