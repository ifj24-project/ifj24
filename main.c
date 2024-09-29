#include "scanner.h"
#include "parser/parser.h"

int main(int argc, char const *argv[])
{
    // printf("hello\n");
    TokenBuffer* t = buffer_ctor();
    Parse_start(t);

    return 0;
}