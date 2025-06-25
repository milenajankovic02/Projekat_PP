// === ast.h ===
#ifndef AST_H
#define AST_H

typedef enum { //ovo nam sluzi kao pokazatelj tipa cvora
    AST_PROGRAM, ///////////  !!!!!!
    AST_DECLARATION, //////////  !!!!!!!
    AST_QUERY_DECLARATION,
    AST_RESULT_OF_QUERY,
    AST_COMMANDS,
    AST_EXEC, //////// !!!!!
    AST_IF,  ////////  !!!!!
    AST_FOR, /////////  !!!!!!!
    AST_ASSIGN, //////// !!!!!
    AST_QUERY,
    AST_TERM,

    AST_OPERATOR_TERM, ////////    !!!!!!!1!
    AST_DIRECTIVE, //////////   !!!!!!!!!!!
    AST_OR,  ///////            !!!!!!!
    AST_BIN_OP,   
    AST_JUXTAPOSITION,   ///////// !!!!!!!

    AST_LIST_OF_QUERIES,
    AST_QUERY_LIST,
    AST_IDENTIFIER,  ///////////    !!!!!!
    AST_STRING_LITERAL, /////////  !!!!!!!
    AST_STRING,
    AST_ASSIGN_SET_OP, /////// !!!!!
    AST_END, //////  !!!!!!
    AST_IN, /////////  !!!!!!!
    AST_BEGIN, /////  !!!!!!!

    AST_SEQUENCE, /////////     !!!!!!!!
   
    AST_COND_EMPTY, ///////      !!!!!!!
    AST_COND_URL_EXISTS, /////    !!!!!!!!!
    AST_COND_NOT_EMPTY, ////////  !!!!!!!!

    AST_OPERATOR, /////   !!!!!!!
    AST_SET_OPERATOR ///////  !!!!!!
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
        char op;
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
            struct Node* name;
            struct Node* value;
        } declaration;

        //for petlja: FOR x IN list BEGIN ...
        struct {
            struct Node* iterator;
            struct Node* in;
            struct Node* query_list;
            struct Node* begin;
            struct Node* body;
            struct Node* end;
        } for_loop;

        //if naredba: IF cond BEGIN comands END
        struct {
            struct Node* condition;
            struct Node* begin;
            struct Node* body;
            struct Node* end;
        } if_command;

        // //res = res ++ item
        // struct {
        //     char* left;
        //     char* op; // npr. "++"
        //     char* right;
        // } set_operation;

        //EXEC: res = EXEC y;
        struct {
            struct Node* res;
            struct Node* exec_target;
        } assign_exec;

        //EXEC: res = ;
        struct {
            struct Node* res;
            struct Node* operator1;
            struct Node* op;
            struct Node* operator2;
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
        
        struct {
            struct Node* exec_i;
        } exec_identifier;

        struct {
            struct Node* identifier;
        } create_condition;
        
        struct {
            struct Node* identifier;
            struct Node* url;
        } create_condition_url;
        
    };
} Node;




//definišemo deklaracije funkcija za kreiranje čvorova različitih tipova
#define CREATE_NODE() ((Node *)malloc(sizeof(Node)))
#define SET_NODE_TYPE(NODE, TYPE) (NODE)->type = TYPE

Node* create_program(Node* declarations, Node* commands);
Node* create_program_d(Node* declarations);
Node* create_declaration(Node* name, Node* value);
Node* create_declaration_roq(Node* name);
Node* create_identifier(char* name);
Node* create_string_literal(char* name);
Node* create_for(Node* iterator, Node* in, Node* list, Node* begin, Node* body, Node* end);
Node* create_if(Node* condition, Node* begin, Node* body, Node* end);
Node* create_assign_command_exec(Node* res, Node* name);
Node* create_assign_command_op(Node* res, Node* operator1, Node* op, Node* operator2);
Node* create_condition_empty(Node* identifier);
Node* create_condition_not_empty(Node* identifier) ;
Node* create_condition_url_exists(Node* identifier, Node* url_string);
Node* create_or(Node* left, Node* right);
Node* create_juxtaposition(Node* first, Node* second);
Node* create_unary_op(char op, Node* operand);
Node* create_directive(char* key, char* value) ;
Node* create_end();
Node* create_in();
Node* create_begin();
Node* create_exec(Node* name);
Node* create_sequence(Node *first, Node *second); 
Node* create_operator(char* c); //ne treba mi ovo, ne koristim ga jer operatori obicni ne trebaju da se predaju kao cvorovi jer nemaju svoje semanticko znacenje
Node* create_set_operator(char* c);
void print_indent(int level);



void print_node_info(int level, const char* label, const char* value);
void print_ast_helper(Node* node, int level);
void print_ast(Node* root); 

char* my_strdup(const char* s);





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
;