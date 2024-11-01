#include "expressionparse.h"

Node * Parse_expression(TokenBuffer* token){
    // shift == 1 == <
    // reduce == 0 == >
    // error == -1
    // accept == 2

    // int last_prec_table[P_all_types][P_all_types] = {
    //     {-1, -1, -1, -1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 0},
    //     {-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 1, 1, -1, -1, -1, -1, -1, -1, 0, 0},
    //     {-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, -1, -1, -1, -1, 0, 0},
    //     {-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1},
    //     {-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0},
    //     {-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0},
    //     {-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 0, -1},
    //     {-1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0},
    //     {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, -1, 2}
    // };

    int prec_table[P_all_types][P_all_types] = {
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // non-terminal
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0}, // relation ops
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0},
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0},
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0},
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0},
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0},
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0}, // +  -
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0,  0},
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0}, // * /
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,  0},
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1, -1, -1,  0,  0}, // numbers
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1, -1, -1,  0,  0},
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1, -1, -1,  0,  0},
        {-1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1}, // (
        {-1, -1, -1, -1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, -1, -1, -1, -1,  0,  0}, // )
        {-1, -1, -1, -1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1, -1,  2}, // $
    };

    int bracket_cnt = 0;
    PrecStack * stack = prec_stack_init();
    PrecStackItem * input = next_prec_item(token, &bracket_cnt);
    Prectype first_terminal;

    while (true)
    {
        first_terminal = first_terminal_type(stack);

        
        if (prec_table[first_terminal][input->type] == 2)
        {
            Node* ret = OneChildNode_new(Expression_N, stack->top->node);
            prec_stack_free(stack);
            free(input);
            return ret;
        }

        if (prec_table[first_terminal][input->type] == 1) // shift
        {
            prec_push_item(stack, input);
            input = next_prec_item(token, &bracket_cnt);
        }
        else if (prec_table[first_terminal][input->type] == 0) // reduce
        {
            reduce(stack);
        }
        else if (prec_table[first_terminal][input->type] == -1) // bad syntax
        {
            ThrowError(2);
        }
        else
        {   
            // printf("stack type: %d\n", stack->top->type);
            // printf("input type: %d\n", input->type);
            // printf("loop tady %d \n", prec_table[stack->top->type][input->type]);
            ThrowError(2);
        }
    }
}

PrecStackItem * next_prec_item(TokenBuffer * token, int * bracket_cnt){
    Node* node;
    switch (token->first->type)
    {
    case T_Greater:
        consume_buffer(token, 1);
        return create_prec_item(P_greater, NULL);
    
    case T_Great_Eq:
        consume_buffer(token, 1);
        return create_prec_item(P_greatereq, NULL);
    
    case T_Lesser:
        consume_buffer(token, 1);
        return create_prec_item(P_lesser, NULL);

    case T_Less_Eq:
        consume_buffer(token, 1);
        return create_prec_item(P_lessereq, NULL);

    case T_Equal:
        consume_buffer(token, 1);
        return create_prec_item(P_eq, NULL);
    
    case T_Not_Eq:
        consume_buffer(token, 1);
        return create_prec_item(P_noteq, NULL);

    case T_Plus:
        consume_buffer(token, 1);
        return create_prec_item(P_plus, NULL);

    case T_Minus:
        consume_buffer(token, 1);
        return create_prec_item(P_minus, NULL);

    case T_Mul:
        consume_buffer(token, 1);
        return create_prec_item(P_times, NULL);
    
    case T_Div:
        consume_buffer(token, 1);
        return create_prec_item(P_divide, NULL);

    case T_ID:
        node = Parse_id(token);
        return create_prec_item(P_id, node);

    case T_Integer:
        node = IntNode_new(token->first->value.integer);
        consume_buffer(token, 1);
        return create_prec_item(P_int, node);

    case T_Float:
        node = FloatNode_new(token->first->value.Float);
        consume_buffer(token, 1);
        return create_prec_item(P_float, node);

    case T_L_Round_B:
        consume_buffer(token, 1);
        (*bracket_cnt)++;
        return create_prec_item(P_L_RoundB, NULL);

    case T_R_Round_B:
        (*bracket_cnt)--;
        if (token->second->type == T_L_Curly_B || token->second->type == T_Pipe || token->second->type == T_Colon || *bracket_cnt < 0)
        {
            if (*bracket_cnt > -1) ThrowError(2);
            return create_prec_item(P_$, NULL);
        }
        consume_buffer(token, 1);
        return create_prec_item(P_R_RoundB, NULL);
    
    case T_SemiC:
        // consume_buffer(token, 1);
        return create_prec_item(P_$, NULL);

    default:
        ThrowError(2);
        break;
    }
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

PrecStackItem* create_prec_item(Prectype type, Node * data){
    PrecStackItem* item = malloc(sizeof(PrecStackItem));
    if (item == NULL)
    {
        ThrowError(99);
    }

    item->next = NULL;
    item->node = data;
    item->type = type;

    return item;
}

void prec_push_item(PrecStack * stack, PrecStackItem* item){
    item->next = stack->top;

    stack->top = item;
    stack->count++;
    
    return;
}

void prec_stack_pop(PrecStack * stack){
    if (stack->count == 0)
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

    free(stack);

    return;
}

int first_terminal_type(PrecStack* stack){
    PrecStackItem* item = stack->top;

    while (item != NULL)
    {
        if (3 < item->type) return item->type;
        item = item->next;
    }
    
}

void reduce(PrecStack * stack){
    PrecStackItem* item;
    Node* new_node;
    PrecStackItem* top = stack->top;

    switch (stack->top->type)
    {
    case P_term:
    case P_factor:
    case P_compared:
        if (stack->top->next->type == P_greater)
        {   
            if (stack->count < 3){ThrowError(2);}
            new_node = TwoChildNode_new(Greater_N, top->next->next->node, top->node);
            item = create_prec_item(P_expression, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        else if (stack->top->next->type == P_greatereq)
        {   
            if (stack->count < 3){ThrowError(2);}
            new_node = TwoChildNode_new(GreaterEq_N, top->next->next->node, top->node);
            item = create_prec_item(P_expression, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        else if (stack->top->next->type == P_lesser)
        {   
            if (stack->count < 3){ThrowError(2);}
            new_node = TwoChildNode_new(Lesser_N, top->next->next->node, top->node);
            item = create_prec_item(P_expression, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        else if (stack->top->next->type == P_lessereq)
        {   
            if (stack->count < 3){ThrowError(2);}
            new_node = TwoChildNode_new(LesserEq_N, top->next->next->node, top->node);
            item = create_prec_item(P_expression, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        else if (stack->top->next->type == P_eq)
        {   
            if (stack->count < 3){ThrowError(2);}
            new_node = TwoChildNode_new(Eq_N, top->next->next->node, top->node);
            item = create_prec_item(P_expression, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        else if (stack->top->next->type == P_noteq)
        {   
            if (stack->count < 3){ThrowError(2);}
            new_node = TwoChildNode_new(NotEq_N, top->next->next->node, top->node);
            item = create_prec_item(P_expression, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        // else
        // {
        //     item = create_prec_item(P_expression, top->node);
        //     prec_stack_pop(stack);
        //     prec_push_item(stack, item);
        //     break;
        // }


    // case P_term:
        else if (stack->top->next->type == P_plus)
        {   
            if (stack->count < 3){ThrowError(2);}
            new_node = TwoChildNode_new(Plus_N, top->next->next->node, top->node);
            item = create_prec_item(P_compared, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        else if (stack->top->next->type == P_minus)
        {   
            if (stack->count < 3){ThrowError(2);}
            new_node = TwoChildNode_new(Minus_N, top->next->next->node, top->node);
            item = create_prec_item(P_compared, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        // else
        // {
        //     item = create_prec_item(P_compared, top->node);
        //     prec_stack_pop(stack);
        //     prec_push_item(stack, item);
        //     break;
        // }

    // case P_factor:
        else if (stack->top->next->type == P_times)
        {   
            if (stack->count < 3){ThrowError(2);}
            new_node = TwoChildNode_new(Times_N, top->next->next->node, top->node);
            item = create_prec_item(P_term, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        else if (stack->top->next->type == P_divide)
        {   
            if (stack->count < 3){ThrowError(2);}
            new_node = TwoChildNode_new(Divide_N, top->next->next->node, top->node);
            item = create_prec_item(P_term, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        // else
        // {
        //     item = create_prec_item(P_term, top->node);
        //     prec_stack_pop(stack);
        //     prec_push_item(stack, item);
        //     break;
        // }
        else ThrowError(2);

    case P_id:
    case P_int:
    case P_float:
        item = create_prec_item(P_factor, stack->top->node);
        prec_stack_pop(stack);
        prec_push_item(stack, item);
        break;

    case P_R_RoundB:
        if (stack->count < 3)
        {
            ThrowError(2);
        }

        if (stack->top->next->next->type == P_L_RoundB)
        {
            item = create_prec_item(P_factor, stack->top->next->node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);

            prec_push_item(stack, item);
            break;
        }
        
    default:
        ThrowError(2); // cant reduce
        break;
    }

    return;
}
