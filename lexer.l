%{
#include "parser.tab.h"
#include <string.h>
#include "errors.h"

extern int yylineno;

%}

%option noyywrap
%option yylineno

/* Makroi */
STRING     \"([^\\\"]|\\.)*\"
WORD       [a-zA-Z_][a-zA-Z0-9_]*

%%

"QUERY"                { return T_QUERY; }
"EXEC"                 { return T_EXEC; }
"RESULT_OF_QUERY"      { return T_RESULT_OF_QUERY; }
"IF"                   { return T_IF; }
"FOR"                  { return T_FOR; }
"IN"                   { return T_IN; }
"BEGIN"                { return T_BEGIN; }
"END"                  { return T_END; }
"EMPTY"                { return T_EMPTY; }
"NOT_EMPTY"            { return T_NOT_EMPTY; }
"URL_EXISTS"           { return T_URL_EXISTS; }

"++"                   { yylval.str = strdup(yytext); return SET_OPERATOR; }
"--"                   { yylval.str = strdup(yytext); return SET_OPERATOR; }
"**"                   { yylval.str = strdup(yytext); return SET_OPERATOR; }

"+"                    { yylval.str = strdup(yytext); return OPERATOR; }
"-"                    { yylval.str = strdup(yytext); return OPERATOR; }
"*"                    { yylval.str = strdup(yytext); return OPERATOR; }
"|"                    { return '|'; }

":"                    { return T_COLON; }
"<"                    { return '<'; }
">"                    { return '>'; }
"["                    { return '['; }
"]"                    { return ']'; }
"("                    { return '('; }
")"                    { return ')'; }
","                    { return ','; }
";"                    { return ';'; }
"="                    { return '='; }

{STRING}               {
                          yylval.str = strdup(yytext);
                          return STRING_LITERAL;
                      }

{WORD}                 {
                          yylval.str = strdup(yytext);
                          return IDENTIFIER;
                      }


[ \t\r\n]+             { }


.                     {
    report_lexical_error(yytext, yylineno, 0); // ne znamo kolonu, pa stavimo 0
    return -1;
}

%%
