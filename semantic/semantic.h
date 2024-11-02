#ifndef SEMANTIC_H
#define  SEMANTIC_H

#include "../parser/parser.h"

void semantic_scan(Node* parse_tree, SymbolTable* global_sym_table, String* global_func_key, SymbolTable* local_sym_table);
bool find_ret_statement(Node* node);
bool is_whole_float(double value);
VarType semantic_expr(Node* node, SymbolTable* global_table, SymbolTable* local_table);

#endif
