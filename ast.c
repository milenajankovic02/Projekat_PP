#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* create_ast_node(ASTNodeType type, char* value, ASTNode* left, ASTNode* right, ASTNode* extra) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = left;
    node->right = right;
    node->extra = extra;
    return node;
}

void print_indent(int level) {
    int i;
    for (i = 0; i < level; i++) 
    printf("  ");
}

const char* type_name(ASTNodeType type) {
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
        case AST_STRING: return "StringLiteral";
        case AST_SET_OP: return "SetOperation";
        default: return "Unknown";
    }
}

void print_ast(ASTNode* node, int level) {
    if (!node) return;

    print_indent(level);
    printf("%s", type_name(node->type));
    if (node->value) printf(": %s", node->value);
    printf("\n");

    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
    print_ast(node->extra, level + 1);
}

void free_ast(ASTNode* node) {
    if (!node) return;
    if (node->value) free(node->value);
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->extra);
    free(node);
}
