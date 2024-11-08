#ifndef EXPRESSIONPARSE_H
#define EXPRESSIONPARSE_H

#include "parser.h"

typedef enum Prectype {
    P_expression = 0,
    P_compared = 1,
    P_term = 2,
    P_factor = 3,
    P_greater = 4,
    P_greatereq = 5,
    P_lesser = 6,
    P_lessereq = 7,
    P_eq = 8,
    P_noteq = 9,
    P_plus = 10,
    P_minus = 11,
    P_times = 12,
    P_divide = 13,
    P_id = 14,
    P_int = 15,
    P_float = 16,
    P_L_RoundB = 17,
    P_R_RoundB = 18,
    P_$ = 19,
    P_all_types = 20
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

void expr_wrapper_ThrowError(int code);

PrecStack* prec_stack_init();
void prec_stack_push(PrecStack * stack, Prectype type, Node * data);
PrecStackItem* create_prec_item(Prectype type, Node * data);
void prec_push_item(PrecStack * stack, PrecStackItem* item);
void prec_stack_pop(PrecStack * stack);
void prec_stack_free(PrecStack * stack);

PrecStackItem * next_prec_item(TokenBuffer * token, int* bracket_cnt);

void reduce(PrecStack * stack);

int first_terminal_type(PrecStack* stack);

Node * Parse_expression(TokenBuffer* token);

#endif