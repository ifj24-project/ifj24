#include "expressionparse.h"

Node * Parse_expression(TokenBuffer* token){
    /**
     * placeholder
     */
    return NULL;
}

PrecStack* prec_stack_init(){
    PrecStack* stack = malloc(sizeof(PrecStack));
    if (stack == NULL)
    {
        ThrowError(99);
    }

    stack->count = 0;
    stack->top = NULL;

    prec_stack_push(stack, P_$, NULL);
    return stack;
    
}

void prec_stack_push(PrecStack * stack, Prectype type, Node * data){
    PrecStackItem* item = malloc(sizeof(PrecStackItem));
    if (item == NULL)
    {
        ThrowError(99);
    }

    item->next = stack->top;
    item->node = data;
    item->type = type;

    stack->top = item;
    stack->count++;
    
    return;
}

void prec_stack_pop(PrecStack * stack){
    if (stack->count < 0)
    {
        ThrowError(2);
    }

    PrecStackItem * temp = stack->top;
    stack->top = temp->next;
    free(temp);
    stack->count--;

    return;
}

void prec_stack_free(PrecStack * stack){
    while (stack->count != 0)
    {
        prec_stack_pop(stack);
    }
    
    return;
}

void reduce(PrecStack * stack){

}
