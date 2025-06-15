/* parser.y */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char* s);
int yylex(void);
%}

%union {
    char* str;
}

%token <str> IDENTIFIER
%token <str> STRING_LITERAL
%token <str> SET_OPERATOR
%token <str> OPERATOR
%token <str> VALUE
%token <str> KEY

%token T_QUERY T_EXEC T_RESULT_OF_QUERY
%token T_IF T_FOR T_IN T_BEGIN T_END
%token T_EMPTY T_NOT_EMPTY T_URL_EXISTS
%token T_COLON

%type <str> program

%%

program:
    declarations commands
  | declarations
    ;

declarations:
    declaration
  | declarations declaration
    ;

declaration:
    T_QUERY IDENTIFIER '=' query ';'
  | T_QUERY IDENTIFIER '=' list_of_queries ';'
  | T_RESULT_OF_QUERY IDENTIFIER ';'
    ;

commands:
    command
  | commands command
    ;

command:
    T_EXEC IDENTIFIER ';'
  | T_IF condition T_BEGIN commands T_END
  | T_FOR IDENTIFIER T_IN list_of_queries T_BEGIN commands T_END
  | assign_command ';'
    ;

assign_command:
    IDENTIFIER '=' T_EXEC IDENTIFIER
  | IDENTIFIER '=' IDENTIFIER SET_OPERATOR IDENTIFIER
    ;

condition:
    T_EMPTY '(' IDENTIFIER ')'
  | T_URL_EXISTS '(' IDENTIFIER ',' STRING_LITERAL ')'
  | T_NOT_EMPTY '(' IDENTIFIER ')'
    ;

list_of_queries:
    '[' query_list ']'
    ;

query_list:
    query
  | query_list ',' query
    ;

query:
    '<' terms '>'
    ;

terms:
    term                         // pojedinačan termin
  | terms term                   // juxtaposition — jače vezivanje -> prvo pravilo veci prioritet
  | terms '|' terms              // slabije vezivanje
    ;


term:
    IDENTIFIER
  | directive
  | OPERATOR term
  | '(' terms ')'
    ;

directive:
    KEY T_COLON VALUE
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parser error: %s\n", s);
}
