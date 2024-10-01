#include "scanner.h"
#include "parser/parser.h"

int main(int argc, char const *argv[])
{
    // printf("hello\n");
    TokenBuffer* t = buffer_ctor();
    Node * parse_tree = Parse_start(t);
    printf("%d\n",parse_tree->second->type);
    buffer_dtor(t);

    return 0;
}