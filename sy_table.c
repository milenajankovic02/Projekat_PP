// === symbol_table.c ===
#include "sy_table.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Symbol* head = NULL;




void insert_symbol(const char* name) {
    if (symbol_exists(name)) return; // već postoji

    Symbol* s = malloc(sizeof(Symbol));
    s->name = my_strdup(name);
//    s->type = type;
    s->next = head;
    head = s;
}

int symbol_exists(const char* name) {
    Symbol* current = head;
    while (current) {
        if (strcmp(current->name, name) == 0)
            return 1;
        current = current->next;
    }
    return 0;
}

void print_symbol_table() {
    Symbol* current = head;
    printf("Tabela simbola:\n");
    while (current) {
        printf(" - %s (%s)\n", current->name); //,
               //current->type == SYMBOL_QUERY ? "QUERY" : "RESULT_OF_QUERY");
        current = current->next;
    }
}


void free_symbol_table() {
    while (head) {
        Symbol* tmp = head;
        head = head->next;
        free(tmp->name);
        free(tmp);
    }
}
