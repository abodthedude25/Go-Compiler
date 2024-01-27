%{
#include<stdio.h>
#include<ctype.h>
#include <string.h>
#include<stdlib.h>
#include"ast.h"

// We need to declare / define these functions for bison
void yyerror(char * err);
int yylex(void);

%}

%locations //to use line anc column info
%define parse.error custom //to utilize bison defined error handling
%defines //to produce the .tab.h file

/* YYLVAL */

%union {
    struct ASTNode* myAst; 
    char *sval; 
}

/* Prefix token with T_ */
//%define api.token.prefix{T_}

// precedence according to the Golf spec
%left  OR
%left AND
%left EQUALITY UNEQUAL LT LE GT GE
%left ADD SUB
%left MUL DIV MODULO
%right NOT 
%left UMINUS


/* Define start state */
%start start


// this indicates the return type of the *terminals* to be the type of the
// field `sval` and 'ival' are in the `%union` declaration

%token <sval>  
    NUM 
    ADD "+" 
    MUL "*"
    NULLL
    SUB "-"
    //MINUS "-"
    DIV "/"
    AND "&&"
    OR "||"
    EQUALITY "=="
    LT "<"
    GT ">"
    ASSIGN "="
    UNEQUAL "!="
    LE "<="
    GE ">="
    NOT "!"
    LEFT_BRACKET "("
    LEFT_CURLY "{"
    COMMA ","
    RIGHT_BRACKET ")"
    RIGHT_CURLY "}"
    SEMICOLON 
    COLON ":"
    RETURN "return"
    BREAK "break"
    STR
    ID
    MODULO "%"
    VAR "var"
    FUNC "func"
    EMPTY ""
    IF "if"
    FOR "for"
    ELSE "else"


// this indicates the return type of the *nonterminals* to be the type of the
// field `myAst` in the `%union` declaration
%type <myAst> 
    Type
    Block
    StatementList
    Statement
    Declaration
    VarDecl
    FunctionDecl
    VarSpec
    FunctionName
    Signature
    FunctionBody
    Parameters
    ParameterList
    ParameterDecl
    Operand
    Literal
    OperandName
    PrimaryExpr
    Arguments
    ExpressionList2
    ExpressionList
    Expression
    UnaryExpr
    SimpleStmt
    ReturnStmt
    BreakStmt
    IfStmt
    ForStmt
    //EmptyStmt
    Assignment
    ifstmt2
    //ForStmt2
    SourceFile
    TopLevelDecl
    Result

%%

//all of the actions taken the rules should be self-explanatory, 
//given the high degree of readability/abstraction of each of the functions in ast.h
start : SourceFile { program = $1; }

// SourceFile       : { TopLevelDecl ";" } 
SourceFile      : SourceFile TopLevelDecl SEMICOLON {
                                                        $$ = $1;
                                                        addActual($$, $2);
                                                    }
                | %empty                            {
                                                        $$ = newProg("testFile");
                                                    }
                ;


//Type      = TypeName .
//TypeName  = identifier .
Type      : ID                      {
                                        $$ = newTypeIdLine($1, @1.first_line);
                                    }
            ;

//Block = "{" StatementList "}" 
Block : "{" StatementList "}"       { 
                                        $$ = $2;
                                    }      
      ;

// StatementList = { Statement ";" } .
// Declaration   = VarDecl .
// TopLevelDecl  = Declaration | FunctionDecl .
StatementList : StatementList Statement SEMICOLON   {   
                                                        $$ = $1;
                                                        addActual($$, $2);
                                                    }
            | %empty                                { $$ = newBlockNoStmt(); }
            ;

Declaration   : VarDecl             {
                                        $$ = $1;
                                    }
TopLevelDecl  : Declaration         {
                                        $$ = $1;
                                    }
              | FunctionDecl        {
                                        $$ = $1;
                                    }
              ; 

// VarDecl     : "var" VarSpec 
// VarSpec     : identifier Type 
VarDecl     : VAR VarSpec         {
                                        $$ = $2;
                                    }
            ;

VarSpec     : ID Type               {
                                        $$ = newVarDecl(newIdNewLine($1, @1.first_line), $2, @2.first_line);
                                    }
            ;

// FunctionDecl : "func" FunctionName Signature FunctionBody 
// FunctionName : identifier 
// FunctionBody : Block 
FunctionDecl : FUNC FunctionName Signature FunctionBody         {   
                                                                    $$ = newFuncDecl($2, $3, $4, @1.first_line);
                                                                }
            ;

FunctionName : ID                   {
                                        $$ = newIdNewLine($1, @1.first_line);
                                    }
            ;

FunctionBody : Block                {
                                        $$ = $1;
                                    }
             | SEMICOLON            {
                                        char s[80];
                                        if(!strcmp($1,"\\n")){
                                            sprintf(s, "error: unexpected newline at or near line %d and column %d\n", @1.first_line, @1.first_column);
                                        }
                                        else{
                                            sprintf(s, "error: syntax error on \";\" at or near line %d and column %d\n", @1.first_line, @1.first_column);
                                        }
                                        yyerror(s); 

                                    }
            ;

// Signature      : Parameters [ Result ] 
// Result         : Type 
// Parameters     : "(" [ ParameterList [ "," ] ] ")" 
// ParameterList  : ParameterDecl { "," ParameterDecl } 
// ParameterDecl  : identifier Type 
Signature      : Parameters Result      {
                                            $$ = newSignature($1, $2);
                                        }
                | Parameters            {
                                            $$ = newSignature($1, newTypeIdLine("$void", @1.first_line));
                                        }
                ;

Result         : Type                   {
                                            $$ = $1;
                                        }
                ;

Parameters     : "(" ParameterList "," ")"      {
                                                    $$ = $2;
                                                }  
                | "(" ParameterList ")"         {
                                                    $$ = $2;
                                                }
                | "(" ParameterList SEMICOLON ")"   {
                                                        char s[80];
                                                        if(!strcmp($3,"\\n")){
                                                            sprintf(s, "error: unexpected newline at or near line %d and column %d\n", @1.first_line, @1.first_column);
                                                        }
                                                        else{
                                                            sprintf(s, "error: syntax error on \";\" at or near line %d and column %d\n", @1.first_line, @1.first_column);
                                                        }
                                                        yyerror(s); 
                                                    }
                | "(" ")"                       {
                                                    $$ = newParams();
                                                }   
                ;

ParameterList  :  ParameterDecl                     {
                                                        $$ = newParams();
                                                        addParam($$, $1);
                                                    }
                | ParameterList "," ParameterDecl   {
                                                        $$ = $1;
                                                        addParam($$, $3);
                                                    }
                ;

ParameterDecl   : ID Type                           {
                                                        $$ = newParamDecl(newIdNewLine($1, @1.first_line), $2);
                                                    }
                ;


// Operand     : Literal | OperandName | "(" Expression ")" 
// Literal     : BasicLit 
// BasicLit    : int_lit | string_lit 
// OperandName : identifier 
// PrimaryExpr : Operand | PrimaryExpr Arguments 
Operand     : Literal               {
                                        $$ = $1;
                                    }
            | OperandName           {
                                        $$ = $1;
                                    }
            | "(" Expression ")"    {
                                        $$ = $2;
                                    }
            ;

Literal     : NUM                   {
                                        $$ = newIntBasicLit($1, @1.first_line);
                                    }
            | STR                   {
                                        $$ = newStrBasicLit($1, @1.first_line);
                                    }
            ;

OperandName : ID                    {
                                        $$ = newIdLine($1, @1.first_line);
                                    }
            ;   
PrimaryExpr : Operand               {
                                        $$ = $1;
                                    }
            | PrimaryExpr Arguments { 
                                        $$ = newFuncCall($1, $2, @1.first_line);
                                    }
            ;



// Arguments      : "(" [ ExpressionList [ "," ] ] ")" 
// ExpressionList : Expression { "," Expression } 
Arguments      : "(" ExpressionList2 ")"    {
                                                $$ = $2;
                                            }
                ;

ExpressionList2 : ExpressionList ","        {
                                                $$ = $1;
                                            }
                | ExpressionList            {
                                                $$ = $1;
                                            }
                | ExpressionList SEMICOLON  {
                                                char s[80];
                                                if(!strcmp($2,"\\n")){
                                                    sprintf(s, "error: unexpected newline at or near line %d and column %d\n", @1.first_line, @1.first_column);
                                                }
                                                else{
                                                    sprintf(s, "error: syntax error on \";\" at or near line %d and column %d\n", @1.first_line, @1.first_column);
                                                }
                                                yyerror(s); 
                                            }
                | %empty                    { $$ = NULL; }
                ;

ExpressionList : Expression                     {
                                                    $$ = newActuals();
                                                    addActual($$, $1);
                                                }
                | ExpressionList "," Expression {
                                                    $$ = $1;
                                                    addActual($$, $3);
                                                }
                ;

// Expression : UnaryExpr | Expression binary_op Expression 
// UnaryExpr  : PrimaryExpr | unary_op UnaryExpr 
Expression : UnaryExpr                             {
                                                        $$ = $1;
                                                    }
        
            | Expression "||" Expression            {
                                                        $$ = newBinExp(F_OR, $1, $3, @2.first_line);
                                                    }
            | Expression "&&" Expression            {
                                                        $$ = newBinExp(F_AND, $1, $3, @2.first_line);
                                                    }
            | Expression "==" Expression            {
                                                        $$ = newBinExp(F_EQUALITY, $1, $3, @2.first_line);
                                                    }
            | Expression "!=" Expression            {
                                                        $$ = newBinExp(F_UNEQUAL, $1, $3, @2.first_line);
                                                    }
            | Expression "<" Expression             {
                                                        $$ = newBinExp(F_LT, $1, $3, @2.first_line);
                                                    }
            | Expression "<=" Expression            {
                                                        $$ = newBinExp(F_LE, $1, $3, @2.first_line);  
                                                    }
            | Expression ">" Expression             {
                                                        $$ = newBinExp(F_GT, $1, $3, @2.first_line);
                                                    }
            | Expression ">=" Expression            {
                                                        $$ = newBinExp(F_GE, $1, $3, @2.first_line);
                                                    }
            | Expression "+" Expression             { 
                                                        $$ = newBinExp(F_ADD, $1, $3, @2.first_line);
                                                    }
            | Expression "-" Expression             {
                                                        $$ = newBinExp(F_SUB, $1, $3, @2.first_line);
                                                    }
            | Expression "*" Expression             {
                                                        $$ = newBinExp(F_MUL, $1, $3, @2.first_line);
                                                    }
            | Expression "/" Expression             {
                                                        $$ = newBinExp(F_DIV, $1, $3, @2.first_line);
                                                    }
            | Expression "%" Expression             {
                                                        $$ = newBinExp(F_MODULO, $1, $3, @2.first_line);
                                                    }
            ;

UnaryExpr  : PrimaryExpr                    {
                                                $$ = $1;
                                            }
            | "-" UnaryExpr %prec UMINUS    {
                                                $$ = newUnExp(F_SUB, $2, @1.first_line);
                                            }
            | "!" UnaryExpr                 {
                                                $$ = newUnExp(F_NOT, $2, @1.first_line);
                                            }
            ;
// binary_op  : "||" 
//             | "&&" 
//             | rel_op 
//             | add_op 
//             | mul_op
// rel_op     : "==" 
//             | "!=" 
//             | "<" 
//             | "<=" 
//             | ">" 
//             | ">="
// add_op     : "+"  
//             | "-"
// mul_op     : "*" 
//             | "/" 
//             | "%"
// unary_op   : "-" 
//             | "!"

// Statement :
//     Declaration | SimpleStmt |
//     ReturnStmt | BreakStmt | 
//     Block | IfStmt | ForStmt 
Statement : Declaration             {
                                        $$ = $1;
                                    }
            | SimpleStmt            {
                                        $$ = $1;
                                    }
            | ReturnStmt            {
                                        $$ = $1;
                                    }
            | BreakStmt             {
                                        $$ = $1;
                                    }
            | Block                 {
                                        $$ = $1;
                                    }
            | IfStmt                {
                                        $$ = $1;
                                    }
            | ForStmt               {
                                        $$ = $1;
                                    }
            ;

// SimpleStmt : EmptyStmt | ExpressionStmt | Assignment 
SimpleStmt :  Expression            {
                                        $$ = newExprStmt($1);
                                    }
            | Assignment            {
                                        $$ = $1;
                                    }
            | %empty                { $$ = newEmptyStmt(); }
            ;

// EmptyStmt : 
// EmptyStmt : %empty                  {
//                                         $$ = newEmptyStmt();
//                                     }
//           ;

// // ExpressionStmt : Expression 
// ExpressionStmt : Expression         {
//                                         $$ = newExprStmt($1);
//                                     }

// Assignment : Expression assign_op Expression 
Assignment : Expression "=" Expression       {
                                                $$ = newAssnStmt($1, $3, @2.first_line);
                                            }
            ;

// IfStmt : "if" Expression Block [ "else" ( IfStmt | Block ) ] 
IfStmt  : IF Expression FunctionBody "else" ifstmt2        {
                                                        $$ = newIfElseStmt($2, $3, $5, @1.first_line);
                                                    }
        | IF Expression FunctionBody                {
                                                        $$ = newIfStmt($2, $3, @1.first_line);
                                                    }
        ;

ifstmt2 : IfStmt                            {
                                                $$ = $1;
                                            }
        | Block                      {
                                                $$ = $1;
                                            }
        ;

// ForStmt : "for" [ Condition ] Block 
ForStmt : FOR Expression FunctionBody       {
                                                $$ = newForStmt($2, $3, @1.first_line);
                                            }
        | FOR Block                         {
                                                $$ = newForStmt(newId("true"), $2, @1.first_line);
                                            }
        ;

// ForStmt2 : Expression                       {
//                                                 $$ = $1;
//                                             }
//          | %empty                           { $$ =  newId("true"); }
//          ;

// // Condition : Expression 
// Condition : Expression                      {
//                                                 $$ = $1;
//                                             }
// ReturnStmt : "return" [ Expression ] 
ReturnStmt : RETURN Expression            {
                                                $$ = newRetExprStmt($2, @1.first_line);
                                            }
            | RETURN                      {
                                                $$ = newRetStmt(@1.first_line);
                                            }  
            ; 

// BreakStmt : "break" 
BreakStmt : BREAK                         {
                                                $$ = newBrkStmt(@1.first_line);
                                            }
          ;

%%




// most of the time, this is passed ``syntax error'', sometimes ``memory
// exhausted''
// For you: `%define parse.error detailed` (or `custom`) to get bette errors.
void yyerror(char* err) {
    // after this returns, the bison parser may:
    //      - may recover from the error and continue parsing (IF the grammar
    //      contains a suitable error rule)
    //      - OTHERWISE, `yyparse` returns a nozero exit code
    fprintf(stderr, "%s", err);
    exit(1);
}


// This is called when bison detects a syntax error.
// See Section 4.4.2 of the bison manual
static int yyreport_syntax_error (const yypcontext_t *yyctx) {

    // Grab the string / current location of the unexpected token
    char const *unexpectedToken =
           // if there is no lookahead token (unexpected token), we return the
           // empty string.
        yypcontext_token(yyctx) == YYSYMBOL_YYEMPTY
            ? ""
            : yysymbol_name (yypcontext_token(yyctx));
    YYLTYPE unexpectedTokenLoc = *yypcontext_location(yyctx);

    // Again, you should use your own logging facilities!

    fprintf
        ( stderr
        , "Unexpected token `%s` at line %d. and column %d. "
        , unexpectedToken
        , unexpectedTokenLoc.first_line
        , unexpectedTokenLoc.first_column
        );
    

    // Grab the tokens that bisoin expected (we only grab at most 10 of them)
    int const MAX_EXPECTED_TOKENS = 10;
    yysymbol_kind_t expectedSymbolKinds[MAX_EXPECTED_TOKENS];
    int actualNumberOfExpectedTokens = 
        yypcontext_expected_tokens
            ( yyctx
            , expectedSymbolKinds
            , MAX_EXPECTED_TOKENS
            );

    if (actualNumberOfExpectedTokens < 0) {
        // this may be negative on errors, so we forward such
        // errors to `yyparse`
        fprintf(stderr , "\n");
        return actualNumberOfExpectedTokens;
    }

    // read the documentation for why we should do this..
    if (actualNumberOfExpectedTokens == 0) {
        if (expectedSymbolKinds[0] != YYSYMBOL_YYEMPTY) {
            actualNumberOfExpectedTokens = MAX_EXPECTED_TOKENS;
        }
    }

    // two cases to get the english sentence to sort of make sense with the
    // punctuation i.e., want to have
    // > Expected `<token>`.
    // vs.
    // > Expected `<token>`, `<token>`, ..., or `<token>`.
    //
    // Note the punctuation...
    if (actualNumberOfExpectedTokens == 1) {
        fprintf(stderr, "Expected `%s`.\n", yysymbol_name(expectedSymbolKinds[0]));
    } else if (actualNumberOfExpectedTokens > 1) {
        fprintf(stderr, "Expected ");
        for (int i = 0; i + 1 < actualNumberOfExpectedTokens; ++i)
            fprintf(stderr, "`%s`, ", yysymbol_name(expectedSymbolKinds[i]));

        fprintf
            ( stderr
            , "or `%s`.\n"
            , yysymbol_name(expectedSymbolKinds[actualNumberOfExpectedTokens - 1])
            );
    }

    return 0; //error routine finished successfully, so return 0.
}
