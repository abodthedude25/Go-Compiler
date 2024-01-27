/********************************
 * Example C Lexical Analyzer
 * Written for CPSC 411 Tutorial
 * File: scanner.h
 * Shankar Ganesh
 * Added on: Abdelrahman Abbas
 * *****************************/

#ifndef SCANNER_H
#define SCANNER_H
#endif

#pragma once

#include <stdio.h>
#include <stdlib.h>

// Scanner Tokens
// enum Token
// {   T_NULL,
//     T_ADD,
//     T_SUB,
//     T_MULT,
//     T_DIV,
//     T_AND,
//     T_OR,
//     T_EQUALITY,
//     T_LT,
//     T_GT,
//     T_ASSIGN,
//     T_UNEQUAL,
//     T_LE,
//     T_GE,
//     T_NOT,
//     T_LEFT_BRACKET,
//     T_LEFT_CURLY,
//     T_COMMA,
//     T_RIGHT_BRACKET,
//     T_RIGHT_CURLY,
//     T_SEMICOLON,
//     T_COLON,
//     T_KEYWORD,
//     T_STRING,
//     T_ID,
//     T_NUM,
//     T_MODULO,
// };

// Need to export a few flex functions and variables
// to use them in main.
// yylex: main interface. Returns a token.
extern int yylex();
// Input File descriptor
extern FILE *yyin;
// Line number that flex keeps track for us
extern int lineno;
// Token lexeme
extern char* yytext;


//golf scanner specific for error handling and spec related variables
extern enum yytokentype lastToken;
extern int break_or_return;
extern int add_line;
extern int colno;

enum yytokentype token_setting(enum yytokentype token);
void error(FILE* output, char* message, int line, int col);
void warning(FILE* output, char* message, int line, int col);
