#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum {
    SYMBOL_QUERY,
    SYMBOL_RESULT_OF_QUERY
} SymbolType;

typedef struct Symbol {
    char* name;
    SymbolType type;
    struct Symbol* next;
} Symbol;

void insert_symbol(const char* name, SymbolType type);
int symbol_exists(const char* name);
void print_symbol_table();

#endif
