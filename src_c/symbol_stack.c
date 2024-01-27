
#include "symbol_stack.h"
#include <math.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
extern symbol_table* table; 

void stackInit(){ // initialize the stack
    table = (symbol_table*)malloc(sizeof(symbol_table));
    for (int i = 0; i < INITIAL_STACK_SIZE; i++){
        table->symbol_stack[i] = createScopeTable();
    }
    table->top = -1;
    table->size = INITIAL_STACK_SIZE;
}

void initPredeclare(){ // initialize the pre-declared functions and types and constants(universe block)
    stackInit();
    stackPushTable(); // push the universe block

    //adding int, bool, void, and string to universe block
    ASTNode* astNodeInt = (ASTNode*)malloc(sizeof(ASTNode));
    astNodeInt = newTypeId("int");
    astNodeInt->type_name = F_NUM;
    stackPush("int", astNodeInt);

    ASTNode* astNodeBool = (ASTNode*)malloc(sizeof(ASTNode));
    astNodeBool = newTypeId("bool");
    astNodeBool->type_name = F_BOOL;
    stackPush("bool", astNodeBool);

    ASTNode* astNodeVoid = (ASTNode*)malloc(sizeof(ASTNode));
    astNodeVoid = newTypeId("$void");
    astNodeVoid->type_name = F_VOID;
    stackPush("$void", astNodeVoid);

    ASTNode* astNodeString = (ASTNode*)malloc(sizeof(ASTNode));
    astNodeString = newTypeId("string");
    astNodeString->type_name = F_STRING;
    stackPush("string", astNodeString);

    //adding true and false to universe block
    ASTNode* astNode = (ASTNode*)malloc(sizeof(ASTNode));
    ASTNode* astNode2 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = newId("true");
    astNode2->type_name = F_BOOL;
    astNode = newVarDecl(astNode2, astNodeBool, 0);
    astNode->type_name = F_BOOL;
    stackPush("true", astNode);

    astNode = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = newId("false");
    astNode2->type_name = F_BOOL;
    astNode = newVarDecl(astNode2, astNodeBool, 0);
    astNode->type_name = F_BOOL;
    stackPush("false", astNode);

    //adding pre-declared functions to universe block
    astNode = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = (ASTNode*)malloc(sizeof(ASTNode));
    ASTNode* astNode3 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = newParams();
    astNode2->children[0] = newParamDecl(newId("i"), astNodeInt);
    astNode2->children_index++;
    astNode3 = newSignature(astNode2, astNodeVoid);
    astNode = newFuncDecl(newId("printi"), astNode3, NULL, 0);
    astNode->codeGenLabel = "Lprinti";
    stackPush("printi", astNode);

    astNode = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode3 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = newParams();
    astNode2->children[0] = newParamDecl(newId("c"), astNodeInt);
    astNode2->children_index++;
    astNode3 = newSignature(astNode2, astNodeVoid);
    astNode = newFuncDecl(newId("printc"), astNode3, NULL, 0);
    astNode->codeGenLabel = "Lprintc";
    stackPush("printc", astNode);

    astNode = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode3 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = newParams();
    astNode2->children[0] = newParamDecl(newId("b"), astNodeBool);
    astNode2->children_index++;
    astNode3 = newSignature(astNode2, astNodeVoid);
    astNode = newFuncDecl(newId("printb"), astNode3, NULL, 0);
    astNode->codeGenLabel = "Lprintb";
    stackPush("printb", astNode);

    astNode = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode3 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = newParams();
    astNode2->children[0] = newParamDecl(newId("s"), astNodeString);
    astNode2->children_index++;
    astNode3 = newSignature(astNode2, astNodeVoid);
    astNode = newFuncDecl(newId("prints"), astNode3, NULL, 0);
    astNode->codeGenLabel = "Lprints";
    stackPush("prints", astNode);

    astNode = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode3 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = newParams();
    astNode2->children[0] = newParamDecl(newId("i"), astNodeInt);
    astNode2->children_index++;
    astNode3 = newSignature(astNode2, astNodeVoid);
    astNode = newFuncDecl(newId("printi"), astNode3, NULL, 0);
    astNode->codeGenLabel = "Lprinti";
    stackPush("printi", astNode);

    astNode = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode3 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = newParams();
    astNode2->children[0] = newParamDecl(newId("s"), astNodeString);
    astNode2->children_index++;
    astNode3 = newSignature(astNode2, astNodeInt);
    astNode = newFuncDecl(newId("len"), astNode3, NULL, 0);
    astNode->codeGenLabel = "Llen";
    stackPush("len", astNode);

    astNode = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode3 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = newParams();
    astNode3 = newSignature(astNode2, astNodeInt);
    astNode = newFuncDecl(newId("getchar"), astNode3, NULL, 0);
    astNode->codeGenLabel = "Lgetchar";
    stackPush("getchar", astNode);

    astNode = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode3 = (ASTNode*)malloc(sizeof(ASTNode));
    astNode2 = newParams();
    astNode3 = newSignature(astNode2, astNodeVoid);
    astNode = newFuncDecl(newId("halt"), astNode3, NULL, 0);
    astNode->codeGenLabel = "Lhalt";
    stackPush("halt", astNode);

}

//pushes a new symbol table entry onto the current stack
int stackPush( char* identifier, ASTNode* astNode){
    if (astNode->sym ==NULL){
        astNode->sym = astNode;
    }
    if(scopeTableInsert(identifier, astNode, table->symbol_stack[table->top]) == 1){
        return 1;
    }
    else{
        return 0;
    }
    return 1;
}

//pushes a new scope onto the current stack
//there exists a stack of scopes which is implemented using an array, which resized when needed
//and then each entry of the stack is a hash map with chaining
int stackPushTable(){
    if(table->top == table->size- 1){
        if( realloc(table->symbol_stack, table->size*2) == NULL ){
            perror("realloc");
            return 0;
        }

        for(int i = table->size; i < table->size*2; i++){
            table->symbol_stack[i] = createScopeTable();
        }
        table->size = table->size*2;
    }
    table->top++;
    return 1;
}

//pops the current scope off the stack
int stackPop( ){
    if(table->top == -1){
        return 0;
    }
    scopeTableFree(table->symbol_stack[table->top]);
    table->symbol_stack[table->top] = createScopeTable();
    table->top--;
    return 1;
}

//finds the first instance of the identifier in the stack
ASTNode* stackFind(char* identifier){
    ASTNode* temp = NULL;
    if(identifier == NULL){
        return NULL;
    }
    for(int i = table->top; i >= 0; i--){
        if( (temp = scopeTableLookup(identifier, table->symbol_stack[i])) != NULL){
            return temp;
        }
    }
    return temp;
}

//prints the scopes with indents, only used for debugging
//(without popping the proper scopes after they are gone)
void stackPrint(){
    printf("top of stack index: %d\n", table->top);
    for(int i = table->top; i >= 0; i--){
        scopeTablePrint(table->symbol_stack[i], i);
    }
}

//frees the stack, didnt really use here but copilot gave it to me at the very begining of development
//thought it could be useful for debugging only
void stackFree(){
    for(int i = table->top; i >= 0; i--){
        scopeTableFree(table->symbol_stack[i]);
    }
    free(table);
}

////global variable that counts how many breaks to expect when reaching breakStmt
//could've been based as a function paramter but I thought this would be easier
int breakCount = 0;

// this function recurses through our entire AST and associates each node with its symbol table entry
void associateAst(ASTNode* tree, FILE* out) {
    //
    if (tree != NULL){
        switch (tree->node_type) {
            case Prog:
                //global scope, which linearly runs through the variable and function declerations and adds them to the symbol table
                stackPushTable();
                associateGlobal(tree, out);
                tree->sym = tree; //associate the program node with the program AST node
                break;
            case Stmt:
                switch (tree->kind.stmt) 
                {
                    case Block:
                        //push a new scope onto the stack with new block, not related to funcdecl
                        stackPushTable();
                        for (int i = 0; i < tree->children_size; i++) {
                            associateAst(tree->children[i], out);
                        }
                        stackPop();
                        return;
                        break;
                    case For:
                        breakCount += 1;
                        for(int i = 0; i < tree->children_size; i++){
                            associateAst(tree->children[i], out);
                        }
                        controlFlowCheck(tree);// checks if the for loop has a boolean expression
                        breakCount -= 1;
                        return;
                        break;
                        // if(!breakCheck(tree)){
                        //     errorSemantic("break statement not in loop", tree->line);
                        // }
                    case IfStmt:
                    case IfElseStmt:
                        //checks if the if statement has a boolean expression
                        for(int i = 0; i < tree->children_size; i++){
                            associateAst(tree->children[i], out);
                        }
                        controlFlowCheck(tree);
                        return;
                        break;
                    case Assn:
                        //checks if the assignment is valid
                        for(int i = 0; i < tree->children_size; i++){
                            associateAst(tree->children[i], out);
                        }
                        assignmentCheck(tree);
                        return;
                    case BreakStmt:
                        //checks if the break statement is in a loop
                        if(breakCount <= 0){
                            errorSemantic("break must be inside 'for'", tree->line);
                        }
                        break;
                    case ReturnStmt:
                        // errorSemantic("return must be inside function", tree->line);
                        // break;
                    case Empty:
                    case ExprStmt:
                    case DeclStmt:
                    default:
                        break;
                }
                break;
            case Expr:
                switch (tree->kind.exp) 
                {
                    case Id:
                        //checks if the variable is defined and has a defined type
                        if(isType(tree)){
                            char message[100];
                            sprintf(message, "can't use type '%s' here", typeToStr(tree->type_name));
                            errorSemantic(message, tree->line);
                        }
                    case TypeId:
                        ASTNode* temp = undefiedVar(tree->val.sval, tree->line);
                        if(temp != NULL){
                            tree->type_name = temp->type_name;
                            tree->sym = temp;
                        }
                        break;
                    case BinaryExp:
                        //checks if the binary expression is valid
                        for(int i = 0; i < tree->children_size; i++){
                            associateAst(tree->children[i], out);
                        }
                        binaryCheck(tree);
                        return;
                        break;
                    case FuncCall:
                        //checks if the function call is valid
                        for(int i = 1; i < tree->children_size; i++){
                            associateAst(tree->children[i], out);
                        }
                        funcCallCheck(tree);
                        return;
                        break;
                    case BasicLit:
                        //checks if the basic literal is in the correct range
                        intRangeCheck(tree, 0);
                        break;
                    case UnaryExp:
                        //checks if the unary expression is valid and if the basic literal is in the correct range
                        if(tree->children[0]->kind.exp == BasicLit){
                            intRangeCheck(tree, 0);
                        }else{
                            associateAst(tree->children[0], out);
                        }
                        unaryCheck(tree);
                        return;
                        break;
                    case Actual:
                    case Actuals:
                    default:
                        break;
                }
                break;
            case Decl:
                switch (tree->kind.decl) 
                {
                    case ParamDecl:
                    case VarDecl:
                        //only adds the variable to the symbol table if it is not already defined
                        if(table->top > 1){
                            redfinedVar(tree->children[0]->val.sval, tree->line);
                            ASTNode* temp = undefiedVar(tree->children[1]->val.sval, tree->children[1]->line);
                            if ( temp->node_type == Decl && temp->kind.decl == FuncDecl){
                                char message[100];
                                sprintf(message, "expected type, got '%s'", tree->children[1]->val.sval);
                                errorSemantic(message, tree->children[1]->line);
                            }
                            else if(temp != NULL){
                                //if the variable is defined, associate the variable with the symbol table entry
                                tree->type_name = temp->type_name;
                                tree->children[0]->type_name = temp->type_name;
                                tree->children[0]->sym = tree->children[0];
                            }
                            stackPush(tree->children[0]->val.sval, tree);
                        }
                        break;
                    case FuncDecl:
                        stackPushTable();
                        //runs through the function decleration and adds the parameters to the symbol table
                        associateAst(tree->children[1], out);
                        for(int i = 0; i < tree->children[2]->children_size; i++){
                            associateAst(tree->children[2]->children[i], out);
                        }
                        //checks if the function has a correct return statement
                        retrurnError(tree);
                        stackPop();
                        return;
                    case Params:
                    case Signature:
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        //recursively runs through the tree
        for (int i = 0; i < tree->children_size; i++) {
            associateAst(tree->children[i], out);
        }
    }
}


void associateGlobal(ASTNode* tree, FILE* out) {
    //runs through the tree and adds the global variables to the symbol table
    for (int i = 0; i < tree->children_size; i++) {
        ASTNode* child = tree->children[i];
        if (child != NULL)
        {
            switch (child->node_type) {
                case Decl:
                    switch (child->kind.decl) 
                    {
                        case VarDecl:
                            child->isGlobal = 1;
                        case ParamDecl:
                            redfinedVar(child->children[0]->val.sval, child->line);
                            ASTNode* temp = undefiedVar(child->children[1]->val.sval, child->children[1]->line);
                            if(temp != NULL){
                                //if the variable is defined, associate the variable with the symbol table entry
                                child->type_name = temp->type_name;
                                child->children[0]->type_name = temp->type_name;
                                child->children[0]->sym = child->children[0];
                            }
                            stackPush(child->children[0]->val.sval, child);
                            break;
                        case FuncDecl:
                            //checks if the function is already defined
                            redfinedVar(child->children[0]->val.sval, child->line);
                            //adds the function to the symbol table and right association to function name
                            child->sym  = child;
                            child->children[0]->sym = child;
                            child->children[1]->children[0]->type_name = strToType(child->children[1]->children[0]->val.sval);
                            stackPush(child->children[0]->val.sval, child);
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }
    //checks if the main function is defined
    ASTNode* temp = stackFind("main");
    if(temp == NULL){
        errorSemantic("missing main() function", 0);
    }
    //checks if the main function has the correct return type and arguments
    else{
        if(strcmp(temp->children[1]->children[0]->val.sval, "$void")){
            errorSemantic("main() can't have a return value", temp->line);
        }
        else if(temp->children[1]->children[1]->children_index > 0){
            errorSemantic("main() can't have arguments", temp->line);
        }
    }

    associateGlobal2(tree);
}

//checks if the global variables are defined correctly after they have been added to scope table
void associateGlobal2(ASTNode* tree) {
    for (int i = 0; i < tree->children_size; i++) {
        ASTNode* child = tree->children[i];
        if (child != NULL)
        {
            switch (child->node_type) {
                case Decl:
                    switch (child->kind.decl) 
                    {
                        case VarDecl:
                            // checks if the variable is defined correctly
                            ASTNode* temp = scopeTableLookup(child->children[1]->val.sval, table->symbol_stack[table->top]);
                            if(temp != NULL && temp->node_type == Decl && temp->kind.decl == VarDecl){
                                char message[100]; 
                                sprintf(message, "expected type, got '%s'", child->children[1]->val.sval);
                                errorSemantic(message, child->children[1]->line);
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }

}


int characterCmp(char str[], int negative){
    //checks if the string is a number that is out of range
    char *endptr;
    long result = strtol(str, &endptr, 10);

    // Check for various possible errors including if the strtol did indeed convert the string to a number
    if (endptr == str || ((result == LONG_MAX || result == LONG_MIN) && errno == ERANGE)){
        printf("No digits were found\n");
        return 0;    
    }
    if( result > 2147483647 && negative == 0){
        return 0;
    }
    else if ( result > 2147483648 && negative == 1){
        return 0;
    }
    return 1;
}

void intRangeCheck(ASTNode* tree, int negative){
    int line = tree->line;
    //checks if the integer is out of range
    if(tree->node_type == Expr && tree->kind.exp == BasicLit && tree->type_name == F_NUM){
        if( characterCmp(strdup(tree->val.sval), negative) == 0){
            errorSemantic("integer literal out of range ", line);
        }
    }
    else if(tree->kind.exp == UnaryExp && tree->val.op == F_SUB){
        //if its unary, check the child and negate the negative value
        intRangeCheck(tree->children[0], negative ? 0 : 1);
    }
}

//
void redfinedVar(char* identifier, int yylineno){
    //checks if the variable is already defined in current scope
    if( scopeTableLookup(identifier, table->symbol_stack[table->top]) != NULL){
        char message[100];
        sprintf(message, "Redefined variable/function '%s'", identifier);
        errorSemantic(message, yylineno);
    }
}

ASTNode* undefiedVar(char* identifier, int yylineno){
    ASTNode* temp = NULL;
    //checks if the variable is defined at any scope
    if((temp = stackFind(identifier)) == 0 && strcmp(identifier, "$void") != 0 && strcmp(identifier, "true") != 0 && strcmp(identifier, "false") != 0){
        char message[100];
        sprintf(message, "unknown name '%s'", identifier);
        errorSemantic(message, yylineno);
    }

    return temp;
}

//checks if the function is called correctly with right parameters and number of parameters
int funcCallCheck(ASTNode* tree){
    isFunctionCall(tree);
    ASTNode* temp = stackFind(tree->children[0]->val.sval);

    tree->children[0]->sym = temp->sym;
    if(temp->children[1]->children[1]->children_index == 0 && tree->children[1] == NULL){
        tree->type_name = temp->children[1]->children[0]->type_name;
        tree->children[0]->type_name = temp->children[1]->children[0]->type_name;
        return 1;
    }

    int line = tree->line;
    char* func_name = tree->children[0]->val.sval;
    if(temp->children[1]->children[1]->children_index != tree->children[1]->children_index ){
        char message[100];
        sprintf(message, "wrong number of arguments to function '%s'", func_name);
        errorSemantic(message, line);
    }

    ASTNode* temp1 = temp->children[1]->children[1];
    ASTNode* tree1 = tree->children[1];
    
    for(int i = 0; i < tree1->children_index; i++){
        if(tree1->children[i]->type_name != strToType(temp1->children[i]->children[1]->val.sval)){
            char message[100];
            sprintf(message, "wrong type of arguments to function '%s'", func_name);
            fprintf(stderr, "line %d: %s  %s", i, typeToStr(tree1->children[i]->type_name) ,typeToStr(temp1->children[i]->children[0]->type_name));
            errorSemantic(message, line);
        }
    }
    tree->type_name = strToType(temp->children[1]->children[0]->val.sval);
    tree->children[0]->type_name = strToType(temp->children[1]->children[0]->val.sval);
    return 1;
}

//
void unaryCheck(ASTNode* tree){
    if(tree->kind.exp != UnaryExp){
        return;
    }
    //checks if the unary expression is used correctly, and not a type
    if (isType(tree->children[0])){
        char message[100];
        sprintf(message, "can't use type '%s' here", typeToStr(tree->children[0]->type_name));
        errorSemantic(message, tree->line);
    }
    //if unary is '-' then following it must be a number
    if (tree->val.op == F_SUB && tree->children[0]->type_name != F_NUM){
        errorSemantic("operand type mismatch for '-'", tree->line);
    }
    //if unary is '!' then following it must be a boolean
    else if (tree->val.op == F_NOT && tree->children[0]->type_name != F_BOOL){
        fprintf(stderr, "%s ", typeToStr(tree->children[0]->type_name));
        errorSemantic("operand type mismatch for '!'", tree->line);
    }
    else{
        //if the unary is used correctly, then the type of the expression is the same as the type of the child
        tree->type_name = tree->children[0]->type_name;
    }
}

void binaryCheck(ASTNode* tree){
    //checks if the binary expression is used correctly, and not a type
    if (isType(tree->children[0])){
        char message[100];
        sprintf(message, "can't use type '%s' here", typeToStr(tree->children[0]->type_name));
        errorSemantic(message, tree->line);
    }
    if (isType(tree->children[1])){
        char message[100];
        sprintf(message, "can't use type '%s' here", typeToStr(tree->children[1]->type_name));
        errorSemantic(message, tree->line);
    }
    //checks correct depending on what operator is being used and the type of the children accordingly
    if (tree->val.op == F_LT || tree->val.op == F_GT || tree->val.op == F_LE || tree->val.op == F_GE || tree->val.op == F_EQUALITY 
        || tree->val.op == F_UNEQUAL || tree->val.op == F_AND || tree->val.op == F_OR) {

        if (tree->children[0]->type_name != tree->children[1]->type_name ) {
            char message[100];
            sprintf(message, "operand type mismatch for '%s'", opToString(tree->val.op));
            errorSemantic(message, tree->line);
        }
        else if( (tree->children[0]->type_name == F_BOOL) && (tree->val.op == F_LT || tree->val.op == F_GT || tree->val.op == F_LE || tree->val.op == F_GE)){
            char message[100];
            sprintf(message, "operand type mismatch for '%s'", opToString(tree->val.op));
            errorSemantic(message, tree->line);
        }
        else{
            //if the binary is used correctly, then the type of the expression is the same as the type of the child
            tree->type_name = F_BOOL;
            tree->sym = tree->children[0]->sym;
        }
    }
    else if (tree->val.op == F_ADD || tree->val.op == F_SUB || tree->val.op == F_MUL || tree->val.op == F_DIV || tree->val.op == F_MODULO) {
        if ((tree->children[0]->type_name != tree->children[1]->type_name) || tree->children[0]->type_name != F_NUM || tree->children[1]->type_name != F_NUM) {
            char message[100];
            sprintf(message, "operand type mismatch for '%s'", opToString(tree->val.op));
            errorSemantic(message, tree->line);
        }
        else{
            //if the binary is used correctly, then the type of the expression is the same as the type of the child
            tree->type_name = tree->children[0]->type_name;
            tree->sym = tree->children[0]->sym;
        }
    }
}

// this function was used for debugging while testing for if a for loop has a break statement
int breakCheck(ASTNode* tree){
    if (tree == NULL)
        return 0;

    if(tree->kind.stmt == BreakStmt){
        return 1;
    }
    else{
        for(int i = 0; i < tree->children_size; i++){
            if(breakCheck(tree->children[i])) return 1;
        }
        return 0;
    }
}


ASTNode* returnCheck(ASTNode* tree){
    //checks if the return statement is present in the children of tree
    if(tree == NULL)
        return NULL;
    if(tree->kind.stmt == ReturnStmt){
        return tree;
    }
    else{
        for(int i = 0; i < tree->children_size; i++){
            ASTNode* temp = returnCheck(tree->children[i]);
            if(temp != NULL){
                return temp;
            }
        }
        return NULL;
    }
}


void retrurnError(ASTNode* tree){
    //checks if the return statement is present in the children of tree
    ASTNode* check = returnCheck(tree);
    TypeName type = check == NULL ? F_ERROR : (check->children[0] ? check->children[0]->type_name : F_VOID);

    //if the function is void, then the return statement must not have a value or no return stmt at all
    if(tree->children[1]->children[0]->type_name == F_VOID ){
        if(type != F_VOID && type != F_ERROR)
            errorSemantic("this function can't return a value", check->line);
    }
    //if the function is not void, then the return statement must have a value and the type must match
    else{
        if(type == F_ERROR){
            errorSemantic("no return statement in function", tree->line);
        }
        else if(type == F_VOID){
            errorSemantic("this function must return a value", check->line);
        }
        else if (type != tree->children[1]->children[0]->type_name){
            errorSemantic("returned value has the wrong type", check->line);
        }
    }
}

void assignmentCheck(ASTNode* tree){
    //checks if the assignment is used correctly, and not a type
    if (isType(tree->children[0])){
        char message[100];
        sprintf(message, "can't use type '%s' here", typeToStr(tree->children[0]->type_name));
        errorSemantic(message, tree->line);
    }
    if (isType(tree->children[0])){
        char message[100];
        sprintf(message, "can't use type '%s' here", typeToStr(tree->children[0]->type_name));
        errorSemantic(message, tree->line);
    }

    ASTNode* temp = NULL;
    //checks if the left side of the assignment is a variable, and if the types match
    if (tree->children[0]){
        if (tree->children[0]->type_name != tree->children[1]->type_name) {
            errorSemantic("operand type mismatch for '='", tree->line);
        }
        else if(tree->children[0]->kind.exp == FuncCall){
            errorSemantic("can only assign to a variable", tree->line);
        }
        else if((temp = stackFind(tree->children[0]->val.sval)) && temp->kind.decl == FuncDecl){
            errorSemantic("operand type mismatch for '='", tree->line);
        }
        else if(tree->children[0]->node_type == Expr && tree->children[0]->kind.exp == Id && undefiedVar(tree->children[0]->val.sval, tree->line) == NULL){
            (strcmp(tree->children[0]->val.sval, "true") == 0 || strcmp(tree->children[0]->val.sval, "false") == 0)  ? errorSemantic("can't assign to a constant", tree->line) : 0;   
        }
        else if(tree->children[0]->node_type == Expr && tree->children[0]->kind.exp == BasicLit){
            errorSemantic("can't assign to a constant", tree->line);
        }
    }
}


void controlFlowCheck(ASTNode* tree){
    //checks that the expression based ofr 'for' and 'if' statements are boolean
    if(tree->children[0]->node_type == Expr && tree->children[0]->kind.exp == Id){
        if(strcmp(tree->children[0]->val.sval, "true") != 0 && strcmp(tree->children[0]->val.sval, "false") != 0
        && tree->children[0]->type_name != F_BOOL){
            printf("check");
            char message[100];
            char *control_type = tree->kind.stmt == IfStmt || tree->kind.stmt == IfElseStmt ? "if" : "for";
            sprintf(message, "'%s' expression must be boolean type", control_type);
            errorSemantic(message, tree->children[0]->line);
        }   
    }
    else if (tree->children[0]->type_name != F_BOOL){
        char message[100];
        char *control_type = tree->kind.stmt == IfStmt || tree->kind.stmt == IfElseStmt ? "if" : "for";
        sprintf(message, "'%s' expression must be boolean type", control_type);
        errorSemantic(message, tree->children[0]->line);
    }
    else{
        //if the expression is not an id, then it must be a boolean
        tree->type_name = tree->children[0]->type_name;
        tree->sym = tree->children[0]->sym;
    }
}

int isType(ASTNode* tree){
    //checks if the current node is a an existing and correct type from scope
    if(tree && tree->node_type == Expr && tree->kind.exp != BasicLit && 
    tree->kind.exp != UnaryExp && tree->kind.exp != BinaryExp && tree->kind.exp != FuncCall){
        if(tree->val.sval != NULL){
            ASTNode* temp = stackFind( tree->val.sval);
            if( temp != NULL && temp->node_type == Expr && temp->kind.exp == TypeId){
                return 1;
            }
        }
    }
    return 0;
}

int isFunctionCall(ASTNode* tree){
    //checks if the current node is a an existing and correct function from scope
    if(tree->node_type == Expr && tree->kind.exp == FuncCall){
        ASTNode* temp = stackFind(tree->children[0]->val.sval);
        if( temp != NULL && temp->node_type == Decl && temp->kind.decl == FuncDecl){
            return 1;
        }
    }
    char message[100];
    sprintf(message, "can't call something that isn't a function '%s'", tree->children[0]->val.sval);
    errorSemantic(message, tree->children[0]->line);
    return 0;
}

//error printing for semantic errors
void errorSemantic(char* message, int line){
        if(line != 0){
            fprintf(stderr, "Error: %s at or near line %d\n", message, line);
        }
        else{
            fprintf(stderr, "Error: %s\n", message);
        }
        exit(EXIT_FAILURE);
}