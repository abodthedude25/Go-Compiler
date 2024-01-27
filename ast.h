// /*******************************
//  * Shankar Ganesh
//  * C AST for GoLF
//  * Based on my AST for J- 
//  * written for CPSC 411 Winter 
//  * 2020
//  * utilized by: Abdelrahman Abbas
//********************************/


//the code given here I used from the TA's website, and it is very well 
//written adn readable so i decided to not crowd it uneccesary and trivial comments

#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_CHILDREN 3 

typedef enum
{
    F_ADD, F_SUB, F_MUL, F_DIV, F_MODULO, F_NOT,
    F_LT, F_GT, F_LE, F_GE, F_EQUALITY, F_UNEQUAL,
    F_AND, F_OR
} Oper;

typedef enum 
{
    F_NUM, F_BOOL, F_STRING, F_VOID, F_ERROR
} TypeName;

typedef enum {Prog, Stmt, Expr, Decl} NodeType;
typedef enum {IfStmt, IfElseStmt, Block, Empty, Assn, For, ExprStmt, DeclStmt, 
    BreakStmt, ReturnStmt} StmtType;

typedef enum {Id, TypeId, BasicLit, UnaryExp, BinaryExp, FuncCall, Actual, Actuals} ExprType;
typedef enum {idNew, FuncDecl, VarDecl, ParamDecl, Params, Signature} DeclType;

typedef struct ASTNode 
{
    const char* label;
    struct ASTNode** children;
    // struct ASTNode* next; // or sibling 
    NodeType node_type;
    struct ASTNode* sym;

    union
    {
        StmtType stmt;
        ExprType exp;
        DeclType decl;
    } kind;
    
    union
    {
        Oper op;
        char* sval;
    } val;

    int ival;

    int reg; 
    int constFolded;
    
    char* codeGenLabel;
    char* codeGenLabelEpi;
    char* returnError;
    int isGlobal;
    int offset;
    TypeName type_name;
    int line;
    int children_index;
    int children_size;

}ASTNode;


///////////
extern ASTNode* program;
///////////

inline const char* opToString(Oper op)
{
    switch (op) 
    {
    case F_ADD:       return "+";
    case F_SUB:       return "-";
    case F_MUL:      return "*";
    case F_DIV:       return "/";
    case F_MODULO:       return "%";
    case F_LT:        return "<";
    case F_GT:        return ">";
    case F_LE:       return "<=";
    case F_GE:       return ">=";
    case F_AND:       return "&&";
    case F_OR:       return "||";
    case F_EQUALITY:        return "==";
    case F_UNEQUAL:       return "!=";
    case F_NOT:       return "!";
    default:        return "";
    }
}

inline const char* opToAsm(Oper op){
    switch (op) 
    {
    case F_ADD:       return "add";
    case F_SUB:       return "sub";
    case F_MUL:      return "mul";
    case F_DIV:       return "div";
    case F_MODULO:       return "mod";
    case F_LT:        return "slt";
    case F_GT:        return "sgt";
    case F_LE:       return "sle";
    case F_GE:       return "sge";
    case F_AND:       return "and";
    case F_OR:       return "or";
    case F_EQUALITY:        return "seq";
    case F_UNEQUAL:       return "sne";
    default:        return "";
    }
}

inline const char* typeToStr(TypeName type)
{
    switch (type) 
    {
    case F_NUM:       return "NUM";
    case F_BOOL:      return "BOOL";
    case F_STRING:       return "STRING";
    case F_VOID:      return "VOID";
    case F_ERROR:      return "ERROR";
    default:        return "";
    }
}

inline const TypeName strToType(char* type)
{
    if (strcmp(type, "int") == 0) return F_NUM;
    else if (strcmp(type, "bool") == 0) return F_BOOL;
    else if (strcmp(type, "string") == 0) return F_STRING;
    else if (strcmp(type, "$void") == 0) return F_VOID;
    else if (strcmp(type, "ERROR") == 0) return F_ERROR;
    else return F_ERROR;
}

//--------------------------------
// Program 
// -------------------------------
inline ASTNode* newProg(const char* filename)
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create program node");
        return node;
    }

    node->label = strdup(filename);
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Prog;
    node->children_index = 0;
    return node;
}

//--------------------------------
// Statement 
// -------------------------------
inline ASTNode* newIfStmt(ASTNode* clause, ASTNode* body, int line)
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create IfStmt node");
        return node;
    }

    node->label = "IfStmt;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Stmt;
    node->kind.stmt = IfStmt;
    node->children[0] = clause;
    node->children[1] = body;
    node->line = line;
    node->children_index = 2;
    return node;
}

inline ASTNode* newIfElseStmt(ASTNode* clause, ASTNode* ifbody, ASTNode* elsebody, int line)
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create IfElseStmt node");
        return node;
    }

    node->label = "IfElseStmt;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Stmt;
    node->kind.stmt = IfElseStmt;
    node->children[0] = clause;
    node->children[1] = ifbody;
    node->children[2] = elsebody;
    node->line = line;
    node->children_index = 3;
    return node;
}

inline ASTNode* newBlockStmt(ASTNode* stmt) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create BlockStmt node");
        return node;
    }

    node->label = "Block;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Stmt;
    node->kind.stmt = Block;
    node->children[0] = stmt; // Access siblings using next of this child
    node->children_index = 1;
    return node;
}

inline ASTNode* newBlockNoStmt() 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create BlockStmt node");
        return node;
    }

    node->label = "Block;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Stmt;
    node->kind.stmt = Block;
    node->children_index = 0;
    return node;
}

inline ASTNode* newEmptyStmt() 
{

    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create EmptyStmt node");
        return node;
    }
  
    node->label = "EmptyStmt;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Stmt;
    node->kind.stmt = Empty;
    node->children_index = 0;
    return node;
}

inline ASTNode* newAssnStmt(ASTNode* lhs, ASTNode* rhs, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create AssnStmt node");
        return node;
    }

    node->label = "AssnStmt;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Stmt;
    node->kind.stmt = Assn;
    node->line = line;
    node->children[0] = lhs;
    node->children[1] = rhs;
    node->val.op = F_EQUALITY;
    node->children_index = 2;
    return node;
}

inline ASTNode* newForStmt(ASTNode* cond, ASTNode* body, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create ForStmt node");
        return node;
    }

    node->label = "ForStmt;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Stmt;
    node->kind.stmt = For;
    node->line = line;
    node->children[0] = cond;
    node->children[1] = body;
    node->children_index = 2;
    return node;
}

inline ASTNode* newExprStmt(ASTNode* expr) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create ExprStmt node");
        return node;
    }

    node->label = "ExprStmt;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Stmt;
    node->kind.stmt = ExprStmt;
    node->children[0] = expr;
    node->children_index = 1;
    return node;
}

inline ASTNode* newDeclStmt(ASTNode* decl) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create DeclStmt node");
        return node;
    }

    node->label = "DeclStmt;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Stmt;
    node->kind.stmt = DeclStmt;
    node->children[0] = decl;
    node->children_index = 1;
    return node;
}

inline ASTNode* newBrkStmt(int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create BrkStmt node");
        return node;
    }

    node->label = "BreakStmt;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Stmt;
    node->kind.stmt = BreakStmt;
    node->line = line;
    node->children_index = 0;
    return node;
}

inline ASTNode* newRetStmt(int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create ReturnStmt node");
        return node;
    }

    node->label = "ReturnStmt;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Stmt;
    node->kind.stmt = ReturnStmt;
    node->line = line;
    node->children_index = 0;
    return node;
}

inline ASTNode* newRetExprStmt(ASTNode* expr, int line) 
{
    ASTNode* node = newRetStmt(line);
    node->children[0] = expr;
    node->children_index = 1;
    return node;
}

//--------------------------------
// Expressions 
// -------------------------------
inline ASTNode* newId(char *name) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create ID node");
        return node;
    }

    node->label = "ID;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Expr;
    node->kind.exp = Id;
    node->val.sval = name;
    node->children_index = 0;
    return node;
}


inline ASTNode* newIdLine(char *name, int line) 
{
    ASTNode* node = newId(name);
    node->line = line;
    return node;
}

inline ASTNode* newTypeId(char* name) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create TypeId node");
        return node;
    }

    node->label = "TypeID;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Expr;
    node->kind.exp = TypeId;
    node->val.sval = name;
    node->children_index = 0;
    return node;
}

inline ASTNode* newTypeIdLine(char* name, int line) 
{
    ASTNode* node = newTypeId(name);
    node->line = line;
    return node;
}

inline ASTNode* newIntBasicLit(char* value, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create int literal node");
        return node;
    }

    node->label = "F_NUM;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Expr;
    node->kind.exp = BasicLit;
    node->val.sval = value;
    node->line = line;
    node->type_name = F_NUM;
    node->children_index = 0;
    return node;
}

inline ASTNode* newStrBasicLit(char* value, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create string literal node");
        return node;
    }

    node->label = "F_STRING;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Expr;
    node->kind.exp = BasicLit;
    node->val.sval = value;
    node->line = line;
    node->type_name = F_STRING;
    node->children_index = 0;
    return node;
}

inline ASTNode* newUnExp(Oper op, ASTNode* right, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create UnaryExp node");
        return node;
    }

    node->label = "Unary;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Expr;
    node->kind.exp = UnaryExp;
    node->line = line;
    node->val.op = op;
    node->children[0] = right;
    node->children_index = 1;
    return node;
}

inline ASTNode* newBinExp(Oper op, ASTNode* left, ASTNode* right, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create BinaryExp node");
        return node;
    }

    node->label = "BinaryOp;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Expr;
    node->kind.exp = BinaryExp;
    node->line = line;
    node->val.op = op;
    node->children[0] = left;
    node->children[1] = right;
    node->children_index = 2;
    return node;
}

inline ASTNode* newFuncCall(ASTNode* name, ASTNode* args, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create FuncCall node");
        return node;
    }

    node->label = "FuncCall;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Expr;
    node->kind.exp = FuncCall;
    node->children[0] = name;
    node->children[1] = args;
    node->line = line;
    node->children_index = 2;
    return node;
}

inline ASTNode* newActual(ASTNode* expr) 
{
    
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create Actual node");
        return node;
    }

    node->label = "Actual;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Expr;
    node->kind.exp = FuncCall;
    node->children[0] = expr;
    node->children_index = 1;
    return node;
}

inline ASTNode* newActuals() 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create Actuals node");
        return node;
    }

    node->label = "Actuals;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Expr;
    node->kind.exp = Actuals;
    node->children_index = 0;
    return node;
}

//--------------------------------
// Declaratations
// -------------------------------

inline ASTNode* newIdNew(char *name) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create ID node");
        return node;
    }

    node->label = "newID;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Decl;
    node->kind.decl = idNew;
    node->val.sval = name;
    node->children_index = 0;
    return node;
}

inline ASTNode* newIdNewLine(char *name, int line) 
{
    ASTNode* node = newIdNew(name);
    node->line = line;
    return node;
}

inline ASTNode* newFuncDecl(ASTNode* id, ASTNode* sig, ASTNode* body, int line) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create FuncDecl node");
        return node;
    }

    node->label = "FuncDecl;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Decl;
    node->kind.decl = FuncDecl;
    node->line = line;
    node->children[0] = id;
    node->children[1] = sig;
    node->children[2] = body;
    node->children_index = 3;
    return node;
}

inline ASTNode* newVarDecl(ASTNode* id, ASTNode* type, int line)
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create VarDecl node");
        return node;
    }

    node->label = "VarDecl;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Decl;
    node->kind.decl = VarDecl;
    node->line = line;
    node->children[0] = id;
    node->children[1] = type;
    node->children_index = 2;
    return node;
}

inline ASTNode* newParamDecl(ASTNode *id, ASTNode* type) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create ParamDecl node");
        return node;
    }

    node->label = "Param;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Decl;
    node->kind.decl = ParamDecl;
    node->children[0] = id;
    node->children[1] = type;
    node->children_index = 2;
    return node;
}

inline ASTNode* newParams() 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create Params node");
        return node;
    }

    node->label = "Params;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Decl;
    node->kind.decl = Params;
    node->children_index = 0;
    return node;
}

inline ASTNode* newSignature(ASTNode* params, ASTNode* type) 
{
    ASTNode* node = (ASTNode *) malloc(sizeof(ASTNode));
    if (node == NULL) 
    {
        fprintf(stderr, "Failed to create Signature node");
        return node;
    }

    node->label = "Sig;";
    node->children_size = 3;
    node->children = (ASTNode**)malloc(node->children_size*(sizeof(ASTNode*)));

    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    node->reg = -1;
    node->constFolded = 0;
    node->codeGenLabel = NULL;
    node->node_type = Decl;
    node->kind.decl = Signature;
    node->children[0] = type;
    node->children[1] = params;
    node->children_index = 2;
    return node;
}

// NOTE: Parameters and actuals are done this way to 
// save memory. Most nodes will not have so many children
// and this prevents having a max number of arguments/params/statements in a block

// inline void append(ASTNode* stmt1, ASTNode* stmt2)
// {
//     ASTNode *tmp = stmt1;
//     while (tmp->next != NULL) tmp = tmp->next;
//     tmp->next = stmt2;
//     // return stmt1;
// }

inline void addActual(ASTNode* parent, ASTNode* actual)
{
    if(parent->children_index == parent->children_size){
        parent->children_size *= 2;
        parent->children = (ASTNode**)realloc(parent->children, parent->children_size * sizeof(ASTNode*));
    }
    parent->children[parent->children_index] = actual;
    parent->children_index++;

    // return parent;
}


inline void addParam(ASTNode* parent, ASTNode* param)
{
    if(parent->children_index == parent->children_size){
        parent->children_size *= 2;
        parent->children = (ASTNode**)realloc(parent->children, parent->children_size * sizeof(ASTNode*));
    }
    parent->children[parent->children_index] = param;
    parent->children_index++;

    // return parent;
}

void printTree(ASTNode* tree, FILE* out,int INDENT);

#endif 