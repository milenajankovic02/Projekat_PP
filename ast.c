#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include <unistd.h>

//ne radi mi strdup zbog -std=c99
char* my_strdup(const char* s) {
    if (!s) return NULL;
    char* dup = malloc(strlen(s) + 1);
    if (dup) strcpy(dup, s);
    return dup;
}



//u zavisnosti od levela na kojem se nalazi cvor moramo dodavati razmake
void print_indent(int level) {
    for (int i = 0; i < level; i++) printf("  ");
}

//PROGRAM
Node* create_program(Node* declarations, Node* commands)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_PROGRAM;
    node->program.commands = commands;
    node->program.declarations = declarations;
    return node;
}
Node* create_program_d(Node* declarations)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_PROGRAM;
    node->program.commands = NULL; //nema komandi
    node->program.declarations = declarations;
    return node;
}

//DECLARATION
Node* create_declaration(Node* name, Node* value)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_DECLARATION;
    node->declaration.name = name;
    node->declaration.value = value;
    return node;
}

Node* create_declaration_roq(Node* name)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_DECLARATION;
    node->declaration.name = name;
    node->declaration.value = NULL;
    return node;
}

//COMMANDS
Node* create_identifier(char* name) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_IDENTIFIER;
    node->string_value =my_strdup(name);
    return node;
}
Node* create_string_literal(char* name) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_STRING_LITERAL;
    node->string_value =my_strdup(name);
    return node;
}
Node* create_for(Node* iterator, Node* in, Node* list, Node* begin, Node* body, Node* end) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_FOR;
    node->for_loop.iterator = iterator;
    node->for_loop.in = in;
    node->for_loop.query_list = list;
    node->for_loop.begin = begin;
    node->for_loop.body = body;
    node->for_loop.end = end;
    return node;
}

Node* create_if(Node* condition, Node* begin, Node* body, Node* end)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_IF;
    node->if_command.condition = condition;
    node->if_command.begin = begin;
    node->if_command.body = body;
    node->if_command.end = end;
    return node;
}

//assign command
//PR., temp = EXEC item;
//left = "temp"
//exec_target = "item"
Node* create_assign_command_exec(Node* res, Node* name)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_ASSIGN;
    node->assign_exec.res = res;
    node->assign_exec.exec_target = name;
    return node;
}

//cuvamo rezultat u res
Node* create_assign_command_op(Node* res, Node* operator1, Node* op, Node* operator2)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_ASSIGN_SET_OP;
    node->assign_op.res = res;
    node->assign_op.operator1 = operator1;
    node->assign_op.op = op;
    node->assign_op.operator2 = operator2;
    return node;
}

//CONDITION

Node* create_condition_empty(Node* identifier) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_COND_EMPTY;
    node->create_condition.identifier = identifier;
    return node;
}

Node* create_condition_not_empty(Node* identifier) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_COND_NOT_EMPTY;
    node->create_condition.identifier = identifier;
    return node;
}

Node* create_condition_url_exists(Node* identifier, Node* url_string) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_COND_URL_EXISTS;
    node->create_condition_url.identifier = identifier;
    node->create_condition_url.url = url_string;
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
//ne treba mi 
Node* create_operator(char* op)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_OPERATOR;
    node->string_value =my_strdup(op);
    return node;
}
Node* create_set_operator(char* string)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_SET_OPERATOR;
    node->string_value =my_strdup(string); // sigurnije
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

//DIRECTIVE
Node* create_directive(char* key, char* value) 
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_DIRECTIVE;
    node->directive.key = my_strdup(key);
    node->directive.value = my_strdup(value);
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
Node* create_exec(Node* name)
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_EXEC;
    node->exec_identifier.exec_i = name;
    return node;
}


Node* create_sequence(Node *first, Node *second) //pravljenje sekvenci čvorova
{
    Node* node = malloc(sizeof(Node));
    node->type = AST_SEQUENCE;
    node->sequence.node1 = first;
    node->sequence.node2 = second;
    return node;
}


void print_node_info(int level, const char* label, const char* value) {
    print_indent(level);
    if (value)
        printf("%d. nivo: %s [%s]\n", level, label, value);
    else
        printf("%d. nivo: %s\n", level, label);
}

void print_ast_helper(Node* node, int level) {
    if (!node) return;

    switch (node->type) {
        case AST_PROGRAM:
            print_node_info(level, "PROGRAM", NULL);
            print_ast_helper(node->program.declarations, level + 1);
            print_ast_helper(node->program.commands, level + 1);
            break;
        case AST_DECLARATION:
            print_node_info(level, "DECLARATION", node->declaration.name->string_value);
            print_ast_helper(node->declaration.value, level + 1);
            break; 

        case AST_EXEC:
            print_node_info(level, "EXEC", node->exec_identifier.exec_i->string_value);
            break;

        case AST_IF:
            print_node_info(level, "IF", NULL);
            print_ast_helper(node->if_command.condition, level + 1);

            print_node_info(level + 1, "BEGIN", NULL);
            print_ast_helper(node->if_command.begin, level + 2);

            print_node_info(level + 1, "BODY", NULL);
            print_ast_helper(node->if_command.body, level + 2);

            print_node_info(level + 1, "END", NULL);
            print_ast_helper(node->if_command.end, level + 2);
            break;

        case AST_FOR:
            print_node_info(level, "FOR", NULL);

            print_node_info(level + 1, "ITERATOR", NULL);
            print_ast_helper(node->for_loop.iterator, level + 2);

            print_node_info(level + 1, "IN", NULL);
            print_ast_helper(node->for_loop.query_list, level + 2);

            print_node_info(level + 1, "BEGIN", NULL);
            print_ast_helper(node->for_loop.begin, level + 2); //mogu i bez ovoga posto begin nema sadrzaj

            print_node_info(level + 1, "BODY", NULL);
            print_ast_helper(node->for_loop.body, level + 2);

            print_node_info(level + 1, "END", NULL);
            print_ast_helper(node->for_loop.end, level + 2);
            break;
        
        //res = EXEC var
        case AST_ASSIGN:
            print_node_info(level, "ASSIGN", node->assign_exec.res->string_value);
            print_node_info(level + 1, "EXEC_TARGET", node->assign_exec.exec_target->string_value);
            break;  

        //res = op1 set_op op2
        case AST_ASSIGN_SET_OP:
            print_node_info(level, "ASSIGN_SET_OP", node->assign_op.res->string_value);
            print_ast_helper(node->assign_op.operator1, level + 1);
            print_ast_helper(node->assign_op.op, level + 1);
            print_ast_helper(node->assign_op.operator2, level + 1);
            break;

        //nece mi trebati
        case AST_OPERATOR:
            print_node_info(level, "OPERATION", node->string_value); 
            break;


        case AST_SET_OPERATOR:
            print_node_info(level, "SET_OPERATION", node->string_value);
            break;


        case AST_COND_EMPTY:
        case AST_COND_NOT_EMPTY:
            print_node_info(level, node->type == AST_COND_EMPTY ? "EMPTY" : "NOT_EMPTY", NULL);
            print_ast_helper(node->create_condition.identifier, level + 1);
            break;

        case AST_COND_URL_EXISTS:
            print_node_info(level, "URL_EXISTS", NULL);
            print_ast_helper(node->create_condition_url.identifier, level + 1);
            print_ast_helper(node->create_condition_url.url, level + 1);
            break;

        case AST_IDENTIFIER:
            print_node_info(level, "IDENTIFIER", node->string_value);
            break;

        case AST_STRING_LITERAL:
            print_node_info(level, "STRING_LITERAL", node->string_value);
            break;


        case AST_OPERATOR_TERM: {
            //Pripremi string za ispis operatora (npr. "+")
            char op_str[2] = { node->unaryOp.op, '\0' };
            print_node_info(level, "UNARY_OP", op_str);
            
            //Ispisi operand (npr. "fruit")
            print_ast_helper(node->unaryOp.operand, level + 1);
            break;
        }


        case AST_DIRECTIVE:
            print_node_info(level, "DIRECTIVE", NULL);
            print_node_info(level + 1, "KEY", node->directive.key);
            print_node_info(level + 1, "VALUE", node->directive.value);
            break;


        case AST_JUXTAPOSITION:
            print_node_info(level, "JUXTAPOSITION", NULL);
            print_ast_helper(node->juxtaposition.first, level + 1);
            print_ast_helper(node->juxtaposition.second, level + 1);
            break;

        case AST_OR:
            print_node_info(level, "OR", NULL);
            print_ast_helper(node->binaryOp.left, level + 1);
            print_ast_helper(node->binaryOp.right, level + 1);
            break;

        case AST_SEQUENCE:
            print_node_info(level, "SEQUENCE", NULL);
            print_ast_helper(node->sequence.node1, level + 1);
            print_ast_helper(node->sequence.node2, level + 1);
            break;

        default:
            print_node_info(level, "NEPOZNAT CVOR", NULL);
            break;
    }
}

void print_ast(Node* root) {
    printf("\n--------------- AST ---------------\n");
    print_ast_helper(root, 0);
    printf("-----------------------------------\n");
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