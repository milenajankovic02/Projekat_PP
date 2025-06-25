#ifndef ERRORS_H
#define ERRORS_H

void report_syntax_error(const char* error, int line, int column);
void report_lexical_error(const char* message, int line, int column); //deklaracije funkcija
void report_semantic_error(const char* message, int line, int column);

#endif
