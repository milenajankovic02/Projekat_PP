#include "errors.h"
#include <stdio.h>

#define RED "\x1b[31m" //za boje greški
#define RESET "\x1b[0m"

//funkcija za štampanje sintaksnih grešaka, recimo kada gramatika nije ispoštovana u .y
void report_syntax_error(const char* error, int line, int column) {
    fprintf(stderr, RED "Sintaksna greska: %s na liniji %d, koloni %d\n" RESET, error, line, column);
}

//greška nepravilnog oblika tokena, ili nepoznatog tokena
void report_lexical_error(const char* message, int line, int column) {
    fprintf(stderr, RED "Leksicka greska: %s na liniji %d, koloni %d\n" RESET, message, line, column);
}
