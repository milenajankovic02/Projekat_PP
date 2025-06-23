// === ast.h ===
#ifndef AST_H
#define AST_H

typedef enum { //ovo nam sluzi kao pokazatelj tipa cvora
    AST_PROGRAM, ///////////
    AST_DECLARATION, //////////
    AST_QUERY_DECLARATION,
    AST_RESULT_OF_QUERY,
    AST_COMMANDS,
    AST_EXEC, ////////
    AST_IF,  ////////
    AST_FOR, /////////
    AST_ASSIGN, ////////
    AST_QUERY,
    AST_TERM,
    AST_OPERATOR_TERM, ////////
    AST_DIRECTIVE, //////////
    AST_OR,  ///////
    AST_BIN_OP,   
    AST_JUXTAPOSITION,   /////////
    AST_LIST_OF_QUERIES,
    AST_QUERY_LIST,
    AST_IDENTIFIER,  ///////////
    AST_STRING_LITERAL, /////////
    AST_STRING,
    AST_SET_OP, 
    AST_END, ////// 
    AST_IN, /////////
    AST_BEGIN, /////
    AST_SEQUENCE, /////////
    AST_COND_EMPTY, ///////
    AST_COND_URL_EXISTS, /////
    AST_COND_NOT_EMPTY ////////
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

        //za identificatore, stringove, result of query itd.
        char* string_value;

        //binarne operacije: term | term, ili set_op
        //valjda operacija OR ili +-/* a ne ovo set_op
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
        //nisam ovo ni iskoristila?? dovoljno je binaryOp kad je sve istog tipa?
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

        //EXEC: res = EXEC y;
        struct {
            char* res;
            char* exec_target;
        } assign_exec;

        //EXEC: res = ;
        struct {
            char* res;
            char* operator1;
            char* operator2;
        } assign_op;


        //query: <terms>
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

        struct {
            struct Node* node1;
            struct Node* node2;
        } sequence;

    };
} Node;


Node* create_program(Node* declarations, Node* commands);
Node* create_program_d(Node* declarations);
Node* create_declaration(char* name, Node* value);
Node* create_declaration_roq(char* name);
Node* create_identifier(char* name);
Node* create_string_literal(char* name);
Node* create_for(char* iterator, Node* list, Node* body);
Node* create_if(Node* condition, Node* body);
Node* create_assign_command_exec(char* res, char* name);
Node* create_assign_command_op(char* res, char* operator1, char* operator2);
Node* create_condition_empty(char* identifier);
Node* create_condition_not_empty(char* identifier);
Node* create_condition_url_exists(char* identifier, char* url_string);
Node* create_or(Node* left, Node* right);
Node* create_juxtaposition(Node* first, Node* second);
Node* create_unary_op(char op, Node* operand);
Node* create_directive(char* key, char* value) ;
Node* create_end();
Node* create_in();
Node* create_begin();
Node* create_exec(char* name);
Node* create_sequence(Node *first, Node *second); 









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