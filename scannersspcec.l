/* Added on: Abdelrahman Abbas*/

/* used the following stackoverflow for variadiac function help: */
/* https://stackoverflow.com/questions/7330821/c-variadic-functions-how-to-get-string-from-input*/

%option noyywrap
%option noinput

/* Declarations */
%{
    // Declarations go here
    #include "scanner.h"
    #include "parsespec.tab.h"
    #include <string.h>

    #define YY_USER_ACTION (colno += yyleng);


    // used only to implement the semicolon insertion
    enum yytokentype lastToken = NULLL; 
    
    //makes sure that if we have a return or break as a token it is set to true
    // int break_or_return = 0; 
    
    //keeps track of number manually, as semicolon insertion process complicates it for yylineno
    int lineno = 1; 
    int warning_count = 0; 

    //keeps track of coloumn number of an error or warning
    int colno = 0;
    int previous_match_length = 0;
    // if we insert semicolon before newline then we return before adding to lino, so this is a boolean for this 
    int add_line = 0;

    //used to keep track ofthe last token and return the token enum for printing in main
    enum yytokentype token_setting(enum yytokentype token){
        lastToken = token;
        if (add_line == 1){
            lineno++;
            add_line = 0;
            colno = 0;
        }
        return token;
    }

    //printing error to stderr and exiting falsly
    void error(FILE* output, char* message, int line, int col){
        fprintf(output, "Error: %s at or near line %d and column %d\n", message, line, col);
        exit(EXIT_FAILURE);
    }

    //prints out warning then error after 10 warnings
    void warning(FILE* output, char* message, int line, int col){
        fprintf(output, "Warning: %s at or near line %d and column %d\n", message, line, col);
        warning_count += 1;
        if (warning_count > 10){//more than 10 warning and you get an error
            error(stderr, "too many warnings", line, colno);
        }
    }

    /*all c escape sequences corretly printing*/
    char const* printableChar(char c){
        switch (c)
        {
            case '\a':             return "\\a";
            case '\b':             return "\\b";
            case '\f':             return "\\f";
            case '\n':             return "NUL";
            case '\r':             return "\\r";
            case '\t':             return "\\t";
            case '\v':             return "\\v";
            case '\0':             return "\\0";
            case '\'':             return "\\'";
            case '\"':             return "\\\"";
            case '\?':             return "\\?";
            case '\\':             return "\\\\";
            default://this is also to print non-escape charcters, with processing to return as character pointer
                char* str;
                str = (char *) malloc(2*sizeof(char));//no need to care about dynamic memory freeing as part of specs
                str[0] = c;
                str[1] = '\0';               
                return str;
        }
    }   

%}

/* Definitions here (characters)*/

ascii_char      ([^\n])
ascii_letter    [a-zA-Z]
ascii_digit     [0-9]
letter          ({ascii_letter}|"_")
decimal_digit   {ascii_digit}

int         ({decimal_digit}({decimal_digit}*))
ID          ({letter}({letter}|{ascii_digit})*)

ascii_value         (({ascii_char})|({escaped_char}))
escaped_char        (\\(\\|n|b|f|t|r|\"))

/* Comment State */
%x COMMENT
/* String State */
%x STRING

%% 

 /* Rules here */

<INITIAL><<EOF>>     {  /* makes sure that we are not missing the semicolon insertion for end of file case as with newline*/
                        if ((lastToken == ID) || (lastToken == NUM) || (lastToken == STR) || (lastToken == RIGHT_BRACKET) || (lastToken == RIGHT_CURLY) || (lastToken == RETURN) || (lastToken == BREAK)){
                            yylloc.first_line = lineno; yylloc.first_column = colno;
                            yylval.sval = strdup(yytext);
                            return token_setting(SEMICOLON);
                        }
                        yyterminate();//end the lexing
                    }

[ \t\r]+            ;       /* Ignore whitespace */
\n                  { /* based on the first 4 bullet points for instertion of semicolon for newline*/
                        yylloc.first_line = lineno; yylloc.first_column = colno;
                        if ((lastToken == ID) || (lastToken == NUM) || (lastToken == STR) || (lastToken == RIGHT_BRACKET) || (lastToken == RIGHT_CURLY) || (lastToken == RETURN) || (lastToken == BREAK)){
                            add_line = 1; /* add to linno variable but in main due to return next*/
                            yylloc.first_line = lineno; yylloc.first_column = colno;
                            yylval.sval = strdup("\\n");
                            return token_setting(SEMICOLON);
                        }
                        
                        lineno++;
                        colno = 0;
                        //lastToken = NULLL;
                    }       /* Track line number through flex*/

 /*operators and punctuation, just returning corresponding enum */
"+"         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(ADD); }
"-"         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(SUB); }
"*"         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(MUL); }
"/"         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(DIV); }
"&&"        { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(AND); }
"||"        { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(OR); }
"=="        { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(EQUALITY); }
"<"         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(LT); }
">"         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(GT); }
"="         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(ASSIGN); }
"!="        { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(UNEQUAL); }
"<="        { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(LE); }
">="        { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(GE); }
"!"         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(NOT); }
"("         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(LEFT_BRACKET); }
"{"         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(LEFT_CURLY); }
","         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(COMMA); }
")"         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(RIGHT_BRACKET); }
"}"         { /* adding semicolon as part of the after } in spec*/
              if (lastToken != SEMICOLON && lastToken != NULLL ){
                    unput('}'); /* put back the } to process after*/
                    yylval.sval = ";";
                    return token_setting(SEMICOLON);
              }
              else{
                    yylval.sval = "}";
                    yylloc.first_line = lineno; yylloc.first_column = colno;
                    return token_setting(RIGHT_CURLY); 
              }
            }
";"         { yylval.sval = strdup(yytext); return token_setting(SEMICOLON); }
":"         { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(COLON); }
"%"         { yylval.sval = strdup(yytext); return token_setting(MODULO); }

 /*Keyword*/
 "var"      { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(VAR); }
 "func"     { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(FUNC); }
"break"     { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(BREAK); }
"else"      { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(ELSE); }
"for"       { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(FOR); }
"if"        { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(IF); }
"return"    { yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(RETURN); }

 /*integer and string literal. also, identifier.*/
{ID}        {
                yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(ID);
            }

{int}       {
                yylloc.first_line = lineno; yylloc.first_column = colno; yylval.sval = strdup(yytext); return token_setting(NUM);
            }


 /* taking out the comments without outputting anything like white space above */
"//"            {BEGIN(COMMENT);}
<COMMENT>.      { };
<COMMENT>\n     { unput('\n'); BEGIN(INITIAL);}

 /* checking for non-ascii, end-quote, valid-char, newline, then and bad string escape in that order */
\"                                      {  previous_match_length = 0; yymore(); BEGIN(STRING); }
<STRING>[\x80-\xff]                     {   colno -= yyleng;/*becasue the YY_USER_ACTION runs after the rule definition*/
                                            error(stderr, "skipping non-ASCII input character", lineno, colno); 
                                        }
<STRING>\"                              { 
                                            colno -= previous_match_length; 
                                            yylloc.first_line = lineno; yylloc.first_column = colno;
                                            yylval.sval = strdup(yytext);
                                            BEGIN(INITIAL); 
                                            return token_setting(STR);  
                                        }
<STRING>{ascii_value}                   { colno -= previous_match_length; previous_match_length = yyleng; yymore(); }
<STRING>\n                              {   colno -= yyleng;
                                            error(stderr, "string contains newline", lineno, colno); 
                                        }
<STRING>(\\.)                           {   colno -= yyleng;/*becasue the YY_USER_ACTION runs after the rule definition*/
                                            char s[50];
                                            sprintf(s, "bad string escape %c", yytext[strlen(yytext)-1]);
                                            error(stderr, s, lineno, colno); 
                                        }
<STRING><<EOF>>                         { /* no need to subtract yyleng as this rule detects the string fully until this EOF*/
                                            error(stderr, "string terminated by EOF", lineno, colno); 
                                        }


 /* warnings and errors, with description in the printed string*/

"&"                                       { error(stderr, "bitwise AND not supported in GoLF", lineno, colno); }
"|"                                       { error(stderr, "bitwise OR not supported in GoLF", lineno, colno); }
([\x80-\xff])                             { warning(stderr, "skipping non-ASCII input character", lineno, colno); }
.|\n                                      { char s[50];
                                            sprintf(s, "skipping unknown character %s" , printableChar(yytext[strlen(yytext)-1]));
                                            warning(stderr, s, lineno, colno);
                                          }

%% 
