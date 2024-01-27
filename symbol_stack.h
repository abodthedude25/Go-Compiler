#ifndef SYMBOL_STACK_H
#define SYMBOL_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_array.h"

#define INITIAL_STACK_SIZE 100

typedef struct symbol_table{
    scope_node** symbol_stack[INITIAL_STACK_SIZE];
    int top;
    int size;
} symbol_table;

int stackPush( char* identifier, ASTNode* astNode);
int stackPop( );
ASTNode* stackFind(char* identifier);
void stackPrint();
void stackFree();
void initPredeclare();
void stackInit();
int stackPushTable();
void associateAst(ASTNode* tree, FILE* out);  
void redfinedVar(char* identifier, int yylineno);
void errorSemantic(char* message, int line);
ASTNode* undefiedVar(char* identifier, int yylineno);
void controlFlowCheck(ASTNode* tree);
void binaryCheck(ASTNode* tree);
void unaryCheck(ASTNode* tree);
// void TypeChecking(ASTNode* tree, FILE* out);
int isType(ASTNode* tree);
int isFunctionCall(ASTNode* tree);
int funcCallCheck(ASTNode* tree);
int breakCheck(ASTNode* tree);
ASTNode* returnCheck(ASTNode* tree);
void retrurnError(ASTNode* tree);
void associateGlobal2(ASTNode* tree);
void intRangeCheck(ASTNode* tree, int negative);
int characterCmp(char* str, int negative);
void assignmentCheck(ASTNode* tree);
void associateGlobal(ASTNode* tree, FILE* out);

#endif