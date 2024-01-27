#include "scanner.h"
#include "parsespec.tab.h"
#include "symbol_stack.h"
#include "code_generation.h"

char const* getName(enum yytokentype tok);
ASTNode* program = NULL;
symbol_table* table = NULL;

int main(int argc, char **argv) 
{
    if (argc == 2) 
    {
        yyin = fopen(argv[1], "r");

        if (!yyin) {
            fprintf(stderr, "error: Failed to open file %s\n", argv[1]);
            perror("Error: ");
            return EXIT_FAILURE;
        }
    }
    else { //as per spec we fail if not a single input file is given
        fprintf(stderr, "error: please input a file argument:(");
        return EXIT_FAILURE;
    } 

    // // Start lexer!
    // enum yytokentype tok;

    // while ((tok = yylex()) != 0){
    //     if (tok == SEMICOLON){ //print empty string if semicolon
    //         fprintf(stdout, "%-10s [] @ line %-10d\n", getName(tok), lineno);
    //         if (add_line) {
    //             lineno++;
    //             colno = 0;
    //         }
    //     }
    //     else{//print matched lexeme
    //         fprintf(stdout, "%-10s [%s] @ line %-10d\n", getName(tok), yytext, lineno);
    //     }
    // }

    //continues to parse until EOF
    if (yyparse() !=0) {
        return EXIT_FAILURE;
    }
    //print the ast on the terminal(stdout)
    initPredeclare();
    associateAst(program, stdout);
    stackPop();// global scope
    stackPop();// predeclare scope
    //stack_print();
    //printTree(program, stdout, 0);

    //generate code
    codeGen(program);
    
    printFileContents("./systemCall.s");

    return EXIT_SUCCESS;
}

//used the folowing functon in correspondance witht the commented out lexing while loop
//above to debug input when error arised from parser
char const* getName(enum yytokentype tok){
	switch (tok)
    {
        case ADD:             return "+";
        case SUB:             return "-";
        case MUL:             return "*";
        case DIV:             return "/";
        case AND:             return "&&";
        case OR:              return "||";
        case EQUALITY:        return "==";
        case LT:              return "<";
        case GT:              return ">";
        case ASSIGN:          return "=";
        case UNEQUAL:         return "!=";
        case LE:              return "<=";
        case GE:              return ">=";
        case NOT:             return "!";
        case LEFT_BRACKET:    return "(";
        case LEFT_CURLY:      return "{";
        case COMMA:           return ",";
        case RIGHT_BRACKET:   return ")";
        case RIGHT_CURLY:     return "}";
        case SEMICOLON:       return ";";
        case COLON:           return ":";
        case STR:          return "string";
        case ID:              return "ID";
        case NUM:             return "int";
        case MODULO:          return "%";
        case IF:              return "if";
        case ELSE:            return "else";
        case FOR:             return "for";
        case BREAK:           return "break";
        case RETURN:          return "return";
        case VAR:             return "var";
        case FUNC:            return "func";
        default:                return "";
    }
}
