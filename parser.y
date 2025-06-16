/* parser.y */
%code requires {
    #include "ast.h"
}
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"
#include "ast.h"

extern int yylineno;
int yylex(void);
ASTNode* root = NULL;

void yyerror(const char* s) {
    report_syntax_error(s, yylineno, 0);
}
%}

%union {
    char* str;
    ASTNode* ast;
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

%type <ast> program declarations declaration commands command assign_command condition list_of_queries query_list query terms term directive

%%

program:
  declarations commands {
        root = create_ast_node(AST_PROGRAM, NULL, $1, $2, NULL);
        $$ = root;
    }
  | declarations {
        root = create_ast_node(AST_PROGRAM, NULL, $1, NULL, NULL);
        $$ = root;
    }
    ;

declarations:
    declaration {
        $$ = $1;
    }
  | declarations declaration {
        $$ = create_ast_node(AST_DECLARATION, NULL, $1, $2, NULL);
    }
    ;

declaration:
    T_QUERY IDENTIFIER '=' query ';' {
        $$ = create_ast_node(AST_QUERY_DECLARATION, $2, $4, NULL, NULL);
    }
  | T_QUERY IDENTIFIER '=' list_of_queries ';' {
        $$ = create_ast_node(AST_QUERY_DECLARATION, $2, $4, NULL, NULL);
    }
  | T_RESULT_OF_QUERY IDENTIFIER ';' {
        $$ = create_ast_node(AST_RESULT_OF_QUERY, $2, NULL, NULL, NULL);
    }
    ;

commands:
    command {
        $$ = $1;
    }
  | commands command {
        $$ = create_ast_node(AST_COMMANDS, NULL, $1, $2, NULL);
    }
    ;

command:
    T_EXEC IDENTIFIER ';' {
        $$ = create_ast_node(AST_EXEC, $2, NULL, NULL, NULL);
    }
  | T_IF condition T_BEGIN commands T_END {
        $$ = create_ast_node(AST_IF, NULL, $2, $4, NULL);
    }
  | T_FOR IDENTIFIER T_IN list_of_queries T_BEGIN commands T_END {
        ASTNode* id = create_ast_node(AST_IDENTIFIER, $2, NULL, NULL, NULL);
        $$ = create_ast_node(AST_FOR, NULL, id, $4, $6);
    }
  | assign_command ';' {
        $$ = $1;
    }
    ;

assign_command:
    IDENTIFIER '=' T_EXEC IDENTIFIER {
        ASTNode* target = create_ast_node(AST_IDENTIFIER, $1, NULL, NULL, NULL);
        ASTNode* exec = create_ast_node(AST_EXEC, $4, NULL, NULL, NULL);
        $$ = create_ast_node(AST_ASSIGN, NULL, target, exec, NULL);
    }
  | IDENTIFIER '=' IDENTIFIER SET_OPERATOR IDENTIFIER {
        ASTNode* left = create_ast_node(AST_IDENTIFIER, $3, NULL, NULL, NULL);
        ASTNode* right = create_ast_node(AST_IDENTIFIER, $5, NULL, NULL, NULL);
        ASTNode* op = create_ast_node(AST_SET_OP, $4, left, right, NULL);
        ASTNode* target = create_ast_node(AST_IDENTIFIER, $1, NULL, NULL, NULL);
        $$ = create_ast_node(AST_ASSIGN, NULL, target, op, NULL);
    }
    ;

condition:
    T_EMPTY '(' IDENTIFIER ')' {
        $$ = create_ast_node(AST_TERM, $3, NULL, NULL, NULL);
    }
  | T_URL_EXISTS '(' IDENTIFIER ',' STRING_LITERAL ')' {
        ASTNode* url = create_ast_node(AST_STRING, $5, NULL, NULL, NULL);
        $$ = create_ast_node(AST_TERM, $3, url, NULL, NULL);
    }
  | T_NOT_EMPTY '(' IDENTIFIER ')' {
        $$ = create_ast_node(AST_TERM, $3, NULL, NULL, NULL);
    }
    ;

list_of_queries:
    '[' query_list ']' {
        $$ = $2;
    }
    ;

query_list:
    query {
        $$ = $1;
    }
  | query_list ',' query {
        $$ = create_ast_node(AST_QUERY_LIST, NULL, $1, $3, NULL);
    }
    ;

query:
    '<' terms '>' {
        $$ = create_ast_node(AST_QUERY, NULL, $2, NULL, NULL);
    }
    ;

terms:
   term {
    $$ = $1;
  }
  | terms term {
    $$ = create_ast_node(AST_JUXTAPOSITION, NULL, $1, $2, NULL);
  }
  | terms '|' terms {
    $$ = create_ast_node(AST_OR, NULL, $1, $3, NULL);
  }
  ;

term:
  IDENTIFIER {
        $$ = create_ast_node(AST_TERM, $1, NULL, NULL, NULL);
    }
  | directive {
        $$ = $1;
    }
  | OPERATOR term {
        $$ = create_ast_node(AST_OPERATOR_TERM, $1, $2, NULL, NULL);
    }
  | '(' terms ')' {
        $$ = $2;
    }
    ;

directive:
    KEY T_COLON VALUE {
        ASTNode* key = create_ast_node(AST_IDENTIFIER, $1, NULL, NULL, NULL);
        ASTNode* value = create_ast_node(AST_STRING, $3, NULL, NULL, NULL);
        $$ = create_ast_node(AST_DIRECTIVE, NULL, key, value, NULL);
    }
    ;

%%
