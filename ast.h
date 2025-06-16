#ifndef AST_H
#define AST_H

typedef enum {
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
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    char* value; // za identifikatore, stringove, operator nazive
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* extra; // dodatni parametar (npr. FOR: left=var, right=list, extra=commands)
} ASTNode;

ASTNode* create_ast_node(ASTNodeType type, char* value, ASTNode* left, ASTNode* right, ASTNode* extra);
void print_ast(ASTNode* node, int level);
void free_ast(ASTNode* node);

#endif
