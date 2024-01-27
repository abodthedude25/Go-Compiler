//I have used the following implemention of a symbol table in C++ from 
//https://www.geeksforgeeks.org/cpp-program-to-implement-symbol-table/?ref=rp
//I have modified it to fit our needs

#include "symbol_array.h"

scope_node* createScopeNode(char* identifier, ASTNode* astNode){
    scope_node* new_node = (scope_node*)malloc(sizeof(scope_node));
    strcpy(new_node->identifier, identifier);
    new_node->astNode = astNode;
    new_node->next = NULL;
    return new_node;
}

//takes a string as input and outputs an integer ID using the popular djb2 hash algorithm:
int hashFunction(char *str) {
    long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    hash = hash % INITIAL_ARRAY_SIZE;
    return hash;
}

scope_node** createScopeTable(){
    scope_node** scope_table = (scope_node**)malloc(sizeof(scope_node*)*INITIAL_ARRAY_SIZE);
    for(int i = 0; i < INITIAL_ARRAY_SIZE; i++){
        scope_table[i] = NULL;
    }
    return scope_table;
}

int scopeTableInsert(char* identifier, ASTNode* astNode, scope_node** scope_table){
    int index = hashFunction(identifier);
    scope_node* new_node = createScopeNode(identifier, astNode);
    if(scope_table[index] == NULL){
        scope_table[index] = new_node;
        return 1;
    }
    else{
        scope_node* temp = scope_table[index];
        while(temp->next != NULL){
            if(strcmp(temp->identifier, identifier) == 0) {
                free(new_node);
                return 1;
            }
            temp = temp->next;
        }
        
        if(strcmp(temp->identifier, identifier) == 0) {
            free(new_node);
            return 1;
        }

        temp->next = new_node;
        return 1;
    }
    return 0;
}

ASTNode* scopeTableLookup(char* identifier, scope_node** scope_table){
    int index = hashFunction(identifier);
    scope_node* temp = scope_table[index];
    while(temp != NULL){
        if(strcmp(temp->identifier, identifier) == 0){
            return temp->astNode;
        }
        temp = temp->next;
    }
    return NULL;
}

int scopeTableDelete(char* identifier, scope_node** scope_table){
    int index = hashFunction(identifier);
    scope_node* temp = scope_table[index];
    scope_node* prev = NULL;
    while(temp != NULL){
        if(strcmp(temp->identifier, identifier) == 0){
            if(prev == NULL){
                scope_table[index] = temp->next;
                free(temp);
                return 1;
            }
            else{
                prev->next = temp->next;
                free(temp);
                return 1;
            }
        }
        prev = temp;
        temp = temp->next;
    }
    return 0;
}

void scopeTablePrint(scope_node** scope_table, int scope){
    for(int i = 0; i < INITIAL_ARRAY_SIZE; i++){
        scope_node* temp = scope_table[i];
        while(temp != NULL){
            // Print indentation
            fprintf(stdout, "%*s", scope*4, "");
            printf("Identifier: %s, ASTNode: %s\n", temp->identifier, (temp->astNode != NULL)
                    ?  temp->astNode->label
                    : "NULL");
            temp = temp->next;
        }
    }
}

void scopeTableFree(scope_node** scope_table){
    for(int i = 0; i < INITIAL_ARRAY_SIZE; i++){
        scope_node* temp = scope_table[i];
        while(temp != NULL){
            scope_node* temp2 = temp;
            temp = temp->next;
            free(temp2);
        }
    }
    free(scope_table);
}

