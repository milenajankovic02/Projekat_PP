// === ast.c ===
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

//u zavisnosti od levela na kojem se nalazi cvor moramo dodavati razmake
void print_indent(int level) {
    for (int i = 0; i < level; i++) printf("  ");
}

const char* ast_node_type_to_string(NodeType type) {
    switch (type) {
        case AST_PROGRAM: return "Program";
        case AST_DECLARATION: return "Declaration";
        case AST_QUERY_DECLARATION: return "QueryDeclaration";
        case AST_RESULT_OF_QUERY: return "ResultOfQuery";
        case AST_COMMANDS: return "Commands";
        case AST_EXEC: return "Exec";
        case AST_IF: return "If";
        case AST_FOR: return "For";
        case AST_ASSIGN: return "Assign";
        case AST_QUERY: return "Query";
        case AST_TERM: return "Term";
        case AST_OPERATOR_TERM: return "OperatorTerm";
        case AST_DIRECTIVE: return "Directive";
        case AST_OR: return "Or";
        case AST_JUXTAPOSITION: return "Juxtaposition";
        case AST_LIST_OF_QUERIES: return "ListOfQueries";
        case AST_QUERY_LIST: return "QueryList";
        case AST_IDENTIFIER: return "Identifier";
        case AST_STRING: return "String";
        case AST_SET_OP: return "SetOperator";
        default: return "Unknown";
    }
}


/*
Primjer ulaza 
QUERY basic = <apple banana>;
-Iz gramatike 
declaration -> "QUERY" query_name "=" query ";"
query -> "<" terms ">"
terms -> terms term
term -> TERM

Parser ovo cita tako sto kada vidi "QUERY" ulazi u declaration 
basic -> postaje query_name 
<apple banana> -> ulazi u query
apple i banana su TERM-ovi


Znaci koristi ovaj dio strukture cvora
struct {
    char* name; -> to je query_name 
    struct Node* value; -> ovo je query koji je takodje lijeva strana nekog pravila
} declaration;
Ovom struct Node* value dodjeljuje se query tip cvora, tj., AST_QUERY_DECLARATION
i onda gleda njegovu vrijednost i radi isto sto je i iznad, obradjuje desnu stranu pravila

Drugi primjer sa for zbog boljeg razumijevanja 
FOR item IN [basic, advanced] BEGIN
    temp = EXEC item;
    res = res ++ item;
END

Prepoznaje token FOR pa koristi pravilo ispod:
command → "FOR" identifier "IN" list_of_queries "BEGIN" commands "END"
list_of_queries → "[" query_list "]"
query_list → query | query_list "," query
assign_command → identifier "=" "EXEC" query_name ";"

Kako je ovo 
FOR iterator in LISTA BEGIN ...body... END
na ovaj tip cvora se odnosi dio:
struct {
    char* iterator; 
    struct Node* query_list;
    struct Node* body;
} for_loop;
char* iterator se koristi za item u for-u, to je niz karaktera 
takodje bi mogli da koristimo 
Node* iterator;
ovaj cvor bi bio tipa AST_IDENTIFIER
i on bi imao sam ime sto je fleksibilnije, ali ovo je jednostavnije trenutno
*/

Node* create_directive(char* key, char* value) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_DIRECTIVE;
    node->directive.key = strdup(key);
    node->directive.value = strdup(value);
    return node;
}

Node* create_identifier(char* name) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_IDENTIFIER;
    node->string_value = strdup(name);
    return node;
}

Node* create_binary_op(Node* first_op, Node* second_op)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_OR; //VALJDA????
    node->binaryOp.left = first_op;
    node->binaryOp.right = second_op;
    return node;
}


Node* create_unary_op(char op, Node* operand) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_OPERATOR_TERM;
    node->unaryOp.op = op;
    node->unaryOp.operand = operand;
    return node;
}

Node* create_juxtaposition(Node* first, Node* second)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_JUXTAPOSITION;
    node->juxtaposition.first = first;
    node->juxtaposition.second = second;
    return node;
}


Node* create_declaration(char* name, Node* value)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_DECLARATION;
    node->declaration.name = strdup(name);
    node->declaration.value = value;
    return node;
}


Node* create_for(char* iterator, Node* list, Node* body) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_FOR;
    node->for_loop.iterator = strdup(iterator);
    node->for_loop.query_list = list;
    node->for_loop.body = body;
    return node;
}

Node* create_if(Node* condition, Node* body)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_IF;
    node->if_command.condition = condition;
    node->if_command.body = body;
    return node;
}

Node* create_set_op(char* left, char* op, char* right)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_SET_OP;
    node->set_operation.left = strdup(left);
    node->set_operation.op = op;
    node->set_operation.right = right;
    return node;
}


//PR., temp = EXEC item;
//left = "temp"
//exec_target = "item"

Node* create_exec(char* left, char* exec_target) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_ASSIGN;
    node->assign_exec.left = strdup(left);
    node->assign_exec.exec_target = strdup(exec_target);
    return node;
}

Node* create_query(Node* terms)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_QUERY;
    node->query.terms = terms;
}

Node* create_query_list(Node** queries, int count)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_QUERY_LIST;
    node->query_list.queries = queries; //VALJDA??
    node->query_list.count = count;
}

Node* create_program(Node* declarations, Node* commands)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_PROGRAM;
    node->program.commands = commands;
    node->program.declarations = declarations;
}

//prva verzija
/*
Node* create_ast_node(NodeType type, char* value, Node* left, Node* right, Node* extra) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = left;
    node->right = right;
    node->extra = extra;
    return node;
}


//rekurzivna fja za stampanje stabla 
void print_ast(Node* node, int level) {
    if (!node) return;
    print_indent(level);
    printf("%s", ast_node_type_to_string(node->type));
    if (node->value) printf(" (%s)", node->value);
    printf("\n");
    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
    print_ast(node->extra, level + 1);
}

void free_ast(Node* node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->extra);
    if (node->value) free(node->value);
    free(node);
}
*/