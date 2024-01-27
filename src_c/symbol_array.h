#ifndef SYMBOL_ARRAY_H
#define SYMBOL_ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

#define INITIAL_ARRAY_SIZE 1000

typedef struct scope_node{
    char identifier[100];
    ASTNode* astNode;
    struct scope_node* next;
} scope_node;

scope_node* createScopeNode(char* identifier, ASTNode* astNode);
int hashFunction(char* identifier);
scope_node** createScopeTable();
int scopeTableInsert(char* identifier, ASTNode* astNode, scope_node** scope_table);
ASTNode* scopeTableLookup(char* identifier, scope_node** scope_table);
int scopeTableDelete(char* identifier, scope_node** scope_table);
void scopeTablePrint(scope_node** scope_table, int scope);
void scopeTableFree(scope_node** scope_table);

#endif