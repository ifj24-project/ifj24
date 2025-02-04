/**
* @file main.c
* @author Patrik Mokrusa (xmokrup00)
*
* IFJ24
*
* @brief driver program pro compiler
*/

#include "parser.h"
#include "semantic.h"
#include "generator.h"

int main()
{
    SymbolTable* symbol_table = create_symbol_table(100);

    TokenBuffer* t = buffer_ctor();
    t->sym_table = symbol_table;
    Node * parse_tree = Parse_start(t);
    buffer_dtor(t);

    semantic_scan(parse_tree, symbol_table, NULL, NULL);

    generate(parse_tree);

    free_symbol_table(symbol_table);
    free_parse_tree(parse_tree);
    return 0;
}
