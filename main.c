#include "parser/parser.h"
#include "semantic/semantic.h"

int main()
{
    // SymbolTable* symbol_table = create_symbol_table(20);

    TokenBuffer* t = buffer_ctor();
    // t->sym_table = symbol_table;
    Node * parse_tree = Parse_start(t);
    
    // buffer_dtor(t);
    semantic_scan(parse_tree);

    /**
     * Gregoruv generator tady.
     */

    // free_symbol_table(symbol_table);
    return 0;
}