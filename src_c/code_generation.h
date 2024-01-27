#ifndef SYMBOL_ARRAY_H
#define SYMBOL_ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

#define REGISTERS_AVAILABLE 8
int registers[REGISTERS_AVAILABLE] = {0};
int paramRegisters[4] = {0};

int allocateRegister();
void AssociateNode(ASTNode* node, int allocated);
void parameterRegisters( int num);
void resetParamRegs();
int pushParamRegs();
void popParamRegs(int count);
void freeRegister(int registerNumber);
void emitGlobal(ASTNode* tree);
char* getStringLabel();
char* getLabel();
char* getLabelGlobal();
void prologueFunction(ASTNode* tree);
void epilogueFunction(ASTNode* tree);
void emit(char* instruction);
void emitLabel(char* label);
void emitString(char* string, ASTNode* tree);
void emitError(char* string, ASTNode* tree);
char* id2Reg(char* id);
void codeGen(ASTNode* node);
void codeGenGlobalVar(ASTNode* node);
void codeGenGlobal(ASTNode* node);
void AssociateString(ASTNode* tree);
void codeGenStrings( ASTNode* node);
void codeGenTraverse(ASTNode* tree);
void codeGenConstFolding(ASTNode* tree);
void printFileContents(const char* filename);
void evalConst(int left, int right, Oper op, ASTNode* tree);

#endif