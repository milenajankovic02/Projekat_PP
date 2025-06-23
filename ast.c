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
        case AST_STRING_LITERAL: return "String Literal";
        case AST_END: return "End";
        case AST_IN: return "In";
        case AST_BEGIN: return "Begin";
        case AST_SEQUENCE: return "Sequence";
        case AST_COND_EMPTY: return "Condition Empty";
        case AST_COND_URL_EXISTS: return "Condition URL exists";
        case AST_COND_NOT_EMPTY: return "Condition not empty";

        case AST_STRING: return "String";
        case AST_SET_OP: return "SetOperator";
        default: return "Unknown";
    }
}


//PROGRAM
Node* create_program(Node* declarations, Node* commands)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_PROGRAM;
    node->program.commands = commands;
    node->program.declarations = declarations;
}
Node* create_program_d(Node* declarations)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_PROGRAM;
    node->program.commands = NULL; //nema komandi
    node->program.declarations = declarations;
}

//DECLARATION
Node* create_declaration(char* name, Node* value)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_DECLARATION;
    node->declaration.name = strdup(name);
    node->declaration.value = value;
    return node;
}

Node* create_declaration_roq(char* name)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_DECLARATION;
    node->declaration.name = strdup(name);
    node->declaration.value = NULL;
    return node;
}

//COMMANDS
Node* create_identifier(char* name) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_IDENTIFIER;
    node->string_value = strdup(name);
    return node;
}
Node* create_string_literal(char* name) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_STRING_LITERAL;
    node->string_value = strdup(name);
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

//assign command
//PR., temp = EXEC item;
//left = "temp"
//exec_target = "item"
Node* create_assign_command_exec(char* res, char* name)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_ASSIGN;
    node->assign_exec.res = res;
    node->assign_exec.exec_target = name;
    return node;
}

//cuvamo rezultat u res
Node* create_assign_command_op(char* res, char* operator1, char* operator2)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_ASSIGN;
    node->assign_op.res = res;
    node->assign_op.operator1 = operator1;
    node->assign_op.operator2 = operator2;
    return node;
}

//CONDITION

Node* create_condition_empty(char* identifier) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_COND_EMPTY;
    node->string_value = strdup(identifier);
    return node;
}

Node* create_condition_not_empty(char* identifier) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_COND_NOT_EMPTY;
    node->string_value = strdup(identifier);
    return node;
}

Node* create_condition_url_exists(char* identifier, char* url_string) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_COND_URL_EXISTS;
    node->binaryOp.left = create_string_node(identifier);
    node->binaryOp.right = create_string_node(url_string);
    return node;
}

//TERMS
//ova fja radi kao create_sequence
Node* create_or(Node* left, Node* right)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_OR;
    node->binaryOp.left = left;
    node->binaryOp.right = right;
    return node;
}

//ova fja radi kao create_sequence
Node* create_juxtaposition(Node* first, Node* second)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_JUXTAPOSITION;
    node->juxtaposition.first = first;
    node->juxtaposition.second = second;
    return node;
}


//TERM
Node* create_unary_op(char op, Node* operand)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_OPERATOR_TERM;
    node->unaryOp.op = op;
    node->unaryOp.operand = operand;
    return node;
}

//DIRECTIVE
Node* create_directive(char* key, char* value) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_DIRECTIVE;
    node->directive.key = strdup(key);
    node->directive.value = strdup(value);
    return node;
}


//T_END ima samo tip
Node* create_end() 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_END;
    return node;
}

//T_IN
Node* create_in()
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_IN;
    return node;
}

//T_BEGIN
Node* create_begin()
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_BEGIN;
    return node;
}

//T_EXEC
Node* create_exec(char* name)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_EXEC;
    node->string_value = name;
    return node;
}


Node* create_sequence(Node *first, Node *second) //pravljenje sekvenci čvorova
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_SEQUENCE;
    node->sequence.node1 = first;
    node->sequence.node2 = second;
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