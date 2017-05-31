#ifndef _SCAN_H_
#define _SCAN_H_

#define MAXTOKENLEN 40

extern char tokenString[MAXTOKENLEN+1];
extern char oldTokenString[MAXTOKENLEN+1];

TokenType getToken(void);
TreeNode * parse(void);

#endif
