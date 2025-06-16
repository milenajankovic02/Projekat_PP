#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "ast.h"
#include "errors.h"

extern FILE* yyin;
extern int yyparse();
extern ASTNode* root; // Definisano u parser.y

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Upotreba: %s <ulazni_fajl>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Greška pri otvaranju fajla");
        return 1;
    }

    yyin = file;

    printf("Startujem parser...\n");
    if (yyparse() == 0) {
        printf("Parsiranje uspješno.\n\nAST:\n");
        print_ast(root, 0);
        free_ast(root);
    } else {
        fprintf(stderr, "Parsiranje nije uspjelo.\n");
    }

    fclose(file);
    printf("Gotovo.\n");
    return 0;
}
