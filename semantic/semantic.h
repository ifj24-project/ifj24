#ifndef SEMANTIC_H
#define  SEMANTIC_H

#include "../parser/parser.h"

void semantic_scan(Node* parse_tree, SymbolTable* global_sym_table);

#endif