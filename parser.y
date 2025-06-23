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
Node* root = NULL;

int sytax_error = 0;
void yyerror(const char* s) {
    sytax_error = 1;
    report_syntax_error(s, yylineno, 0);
}
%}

%union {
    char* str;
    Node* node;
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

%type <node> program declarations declaration commands command assign_command condition list_of_queries query_list query terms term directive

%%

program:
  declarations commands {
      if(sytax_error == 0)
      {
        root = create_program($1, $2);
        $$ = root;
      }
      else
      {
        $$ = NULL;
      }
    }
  | declarations {
      if(sytax_error == 0)
      {
        root = create_program_d($1);
        $$ = root;
      }
      else
      {
        $$ = NULL;
      }
    }
    ;

declarations:
    declaration {
        $$ = $1;
    }
  | declarations declaration {
        $$ = create_sequence($1, $2);
    }
    ;


declaration:
    T_QUERY IDENTIFIER '=' query ';' {
      $$ = create_declaration(create_identifier($2), $4);
    }
  | T_QUERY IDENTIFIER '=' list_of_queries ';' {
      $$ = create_declaration(create_identifier($2), $4);
    }
  | T_RESULT_OF_QUERY IDENTIFIER ';' {
      $$ = create_declaration_roq(create_identifier($2));
    }
    ;

commands:
    command {
        //kreiramo sekvencu komandi ako imamo samo jednu komandu
        $$ = $1;
    }
  | commands command {
        //dodajemo novu komandu u sekvencu
        $$ = create_sequence($1, $2); 
    }
    ;

command:
    T_EXEC IDENTIFIER ';' {
        $$ = create_exec(create_identifier($2));
    }
  | T_IF condition T_BEGIN commands T_END {
        $$ = create_if($2, create_begin(), $4, create_end());
    }
  | T_FOR IDENTIFIER T_IN list_of_queries T_BEGIN commands T_END {
        $$ = create_for(create_identifier($2), create_in(), $4, $6, create_end());
    }
  | assign_command ';' {
        $$ = $1;
    }
    ;

//kada promjenljivoj dodjeljujemo rezultat
assign_command:
    IDENTIFIER '=' T_EXEC IDENTIFIER {
        $$ = create_assign_command_exec(create_identifier($1), $4);
    }
  | IDENTIFIER '=' IDENTIFIER SET_OPERATOR IDENTIFIER {
        $$ = create_assign_command_op(create_identifier($1), create_identifier($3), create_identifier($5));
    }
    ;


condition:
    T_EMPTY '(' IDENTIFIER ')' {
      $$ = create_condition_empty(create_identifier($3));
    }
  | T_URL_EXISTS '(' IDENTIFIER ',' STRING_LITERAL ')' {
      $$ = create_condition_url_exists(create_identifier($3), create_string_literal($5));
    }
  | T_NOT_EMPTY '(' IDENTIFIER ')' {
      $$ = create_condition_not_empty(create_identifier($3));
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
        $$ = create_sequence($1, $3);
    }
    ;

query:
    '<' terms '>' {
        $$ = $2
    }
    ;

terms:
   term {
    $$ = $1;
  }
  | terms term {
    $$ = create_juxtaposition($1, $2); 
  }
  | terms '|' terms {
    $$ = create_or($1, $3);
  }
  ;

term:
    IDENTIFIER {
        $$ = create_identifier($1);
    }
  | directive {
        $$ = $1;
    }
  | OPERATOR term {
        $$ = create_unary_op($1, $2)
    }
  | '(' terms ')' {
     $$ = $2; 
    }
  ; 

directive:
    KEY T_COLON VALUE {
        create_directive($1, $3)
    }
    ;

%%
