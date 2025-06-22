// === ast.h ===
#ifndef AST_H
#define AST_H

typedef enum { //ovo nam sluzi kao pokazatelj tipa cvora
    AST_PROGRAM,
    AST_DECLARATION,
    AST_QUERY_DECLARATION,
    AST_RESULT_OF_QUERY,
    AST_COMMANDS,
    AST_EXEC,
    AST_IF,
    AST_FOR,
    AST_ASSIGN,
    AST_QUERY,
    AST_TERM,
    AST_OPERATOR_TERM,
    AST_DIRECTIVE,
    AST_OR,
    AST_JUXTAPOSITION,
    AST_LIST_OF_QUERIES,
    AST_QUERY_LIST,
    AST_IDENTIFIER,
    AST_STRING,
    AST_SET_OP
} NodeType;



typedef struct Node {
    //svako pravilo predstavljeno je kao cvor
    NodeType type; //lijeva strana pravila 
    //sad definisemo vrijednosti za desnu stranu pravila 
    union {
        //za direktivu: filetype:pdf
        struct {
            char* key;
            char* value;
        } directive;

        //za identificatore, stringove, itd.
        char* string_value;

        //binarne operacije: term | term, ili set_op
        struct {
            struct Node* left;
            struct Node* right;
        } binaryOp;

        //unarni operatori: +term, -term, *term
        struct {
            char op;
            struct Node* operand;
        } unaryOp;

        //za JUXTAPOSITION više termina u nizu
        struct {
            struct Node* first;
            struct Node* second;
        } juxtaposition;

        //za deklaraciju: QUERY ime = query
        struct {
            char* name;
            struct Node* value;
        } declaration;

        //for petlja: FOR x IN list BEGIN ...
        struct {
            char* iterator;
            struct Node* query_list;
            struct Node* body;
        } for_loop;

        //if naredba: IF cond BEGIN comands END
        struct {
            struct Node* condition;
            struct Node* body;
        } if_command;

        //res = res ++ item
        struct {
            char* left;
            char* op; // npr. "++"
            char* right;
        } set_operation;

        //EXEC: x = EXEC y;
        struct {
            char* left;
            char* exec_target;
        } assign_exec;

        //query: <term term | term>
        struct {
            struct Node* terms;
        } query;

        //lista upita: [a, b, c]
        struct {
            struct Node** queries;
            int count;
        } query_list;

        //za PROGRAM!!!
        struct {
            struct Node* declarations;
            struct Node* commands;
        } program;
    };
} Node;






/* moj prvi nacin
typedef struct Node {
    NodeType type; //lijeva strana pravila 
    char* value; //vrijednost npr., QUERY q1  ... -> QUERY(type) q1(value)
    struct ASTNode* left; //lijevi sin
    struct ASTNode* right; //desni sin
    struct ASTNode* extra; //ukoliko neka
} ASTNode;

//deklaracije funkcija
ASTNode* create_ast_node(NodeType type, char* value, ASTNode* left, ASTNode* right, ASTNode* extra);
void print_ast(ASTNode* node, int level);
void free_ast(ASTNode* node);
*/



#endif