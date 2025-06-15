#include <stdio.h>

extern FILE* yyin;
int yyparse(void);

int main(int argc, char* argv[]) {
    printf("Startujem parser...\n");

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Ne mogu da otvorim fajl");
            return 1;
        }
    }

    yyparse();
    printf("Gotovo.\n");
    return 0;
}
