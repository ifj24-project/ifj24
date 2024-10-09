#ifndef EXPRESSIONPARSE_H
#define EXPRESSIONPARSE_H

#include "parser.h"

typedef enum Prectype {
    P_$ = 0,
    P_expression = 1,
    P_compared = 2,
    P_term = 3,
    P_factor = 4,
    P_id = 5,
    P_int = 6,
    P_float = 7
} Prectype;

typedef struct PrecStackItem PrecStackItem;

struct PrecStackItem
{
    Prectype type;
    Node* node;
    PrecStackItem * next;
};


typedef struct PrecStack
{
    int count;
    PrecStackItem * top;
} PrecStack;

PrecStack* prec_stack_init();
void prec_stack_push(PrecStack * stack, Prectype type, Node * data);
void prec_stack_pop(PrecStack * stack);
void prec_stack_free(PrecStack * stack);

void reduce(PrecStack * stack);

Node * Parse_expression(TokenBuffer* token);

#endif