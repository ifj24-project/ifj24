/** 
* @file expressionparse.c
* @author Patrik Mokrusa (xmokrup00)
*
* IFJ24
*
* @brief Implementace precedencniho alanyzatoru
*/

#include "expressionparse.h"
#include "error.h"
#include "scanner.h"

/**
 * ERROR HANDLING
 */

PrecStack* EXPRESSION_PREC_STACK = NULL;
PrecStackItem* EXPRESSION_PREC_INPUT = NULL;

void expr_wrapper_ThrowError(int code){
    if (EXPRESSION_PREC_STACK != NULL) prec_stack_free(EXPRESSION_PREC_STACK);
    if (EXPRESSION_PREC_INPUT != NULL) free(EXPRESSION_PREC_INPUT);
    parse_wrapper_ThrowError(code);
}

/**
 * PRECEDENCE PARSER
 */

Node * Parse_expression(TokenBuffer* token){
    // shift == 1 == <
    // reduce == 0 == >
    // error == -1
    // accept == 2

    // tato tabulka odpovida tabulce v dokumentaci
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


    PrecStack * stack;
    if (EXPRESSION_PREC_STACK != NULL) // have 1 stack for recursive calling (needed for error handling)
    {
        stack = EXPRESSION_PREC_STACK;
        prec_stack_push(stack, P_$, NULL);
    }
    else stack = prec_stack_init();


    PrecStackItem * input = next_prec_item(token, &bracket_cnt);
    Prectype first_terminal;

    // error handling
    EXPRESSION_PREC_STACK = stack;
    EXPRESSION_PREC_INPUT = input;

    while (true)
    {
        first_terminal = first_terminal_type(stack);


        if (prec_table[first_terminal][input->type] == 2) // accept
        {
            Node* ret;
            if (stack->top->type == P_$) expr_wrapper_ThrowError(2);
            if (stack->top->node->type == Id_N || stack->top->node->type == FuncCall_N) ret = stack->top->node;
            else ret = OneChildNode_new(Expression_N, stack->top->node);


            if (stack->count < 2) expr_wrapper_ThrowError(2);
            prec_stack_pop(stack);
            prec_stack_pop(stack);

            if (stack->count == 0) {
                free(stack);
                EXPRESSION_PREC_STACK = NULL;
            }

            free(input);
            return ret;
        }

        if (prec_table[first_terminal][input->type] == 1) // shift
        {
            prec_push_item(stack, input);
            input = next_prec_item(token, &bracket_cnt);
            EXPRESSION_PREC_INPUT = input;
        }
        else if (prec_table[first_terminal][input->type] == 0) // reduce
        {
            reduce(stack);
        }
        else if (prec_table[first_terminal][input->type] == -1) // bad syntax
        {
            fprintf(stderr, "bad syntax EXPRESSION\n");
            expr_wrapper_ThrowError(2);
        }
        else
        {
            expr_wrapper_ThrowError(2);
        }
    }
}

/**
 * PRECEDENCE PARSER UTILS
 */

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
        if (token->second->type == T_L_Round_B || token->second->type == T_Dot)
        {
            node = Parse_func_call(token);
            return create_prec_item(P_id, node);
        }

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
        // comma collon fix?
        if (token->second->type == T_L_Curly_B || token->second->type == T_Pipe || token->second->type == T_Comma || *bracket_cnt < 0)
        {
            if (*bracket_cnt > -1) expr_wrapper_ThrowError(2);
            return create_prec_item(P_$, NULL);
        }
        consume_buffer(token, 1);
        return create_prec_item(P_R_RoundB, NULL);

    case T_Comma:
    case T_SemiC:
        // consume_buffer(token, 1);
        return create_prec_item(P_$, NULL);

    default:
        return create_prec_item(P_expression, NULL); // P_exp instead of P_ERROR because non-terminal cant be input
        // cant add P_ERROR, because it breaks realtion expressions with enums
        // expr_wrapper_ThrowError(2);
        return NULL;
        break;
    }
}

PrecStack* prec_stack_init(){
    PrecStack* stack = malloc(sizeof(PrecStack));
    if (stack == NULL)
    {
        expr_wrapper_ThrowError(99);
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
        expr_wrapper_ThrowError(99);
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
        expr_wrapper_ThrowError(99);
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
        expr_wrapper_ThrowError(2);
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

    return 1;
}

void reduce(PrecStack * stack){
    PrecStackItem* item;
    Node* new_node;
    PrecStackItem* top = stack->top;

    switch (stack->top->type)
    {
    // any non-terminal
    case P_term:
    case P_factor:
    case P_compared:
        if (stack->top->next->type == P_greater)
        {
            if (stack->count < 3){expr_wrapper_ThrowError(2);}
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
            if (stack->count < 3){expr_wrapper_ThrowError(2);}
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
            if (stack->count < 3){expr_wrapper_ThrowError(2);}
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
            if (stack->count < 3){expr_wrapper_ThrowError(2);}
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
            if (stack->count < 3){expr_wrapper_ThrowError(2);}
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
            if (stack->count < 3){expr_wrapper_ThrowError(2);}
            new_node = TwoChildNode_new(NotEq_N, top->next->next->node, top->node);
            item = create_prec_item(P_expression, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        else if (stack->top->next->type == P_plus)
        {
            if (stack->count < 3){expr_wrapper_ThrowError(2);}
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
            if (stack->count < 3){expr_wrapper_ThrowError(2);}
            new_node = TwoChildNode_new(Minus_N, top->next->next->node, top->node);
            item = create_prec_item(P_compared, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        else if (stack->top->next->type == P_times)
        {
            if (stack->count < 3){expr_wrapper_ThrowError(2);}
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
            if (stack->count < 3){expr_wrapper_ThrowError(2);}
            new_node = TwoChildNode_new(Divide_N, top->next->next->node, top->node);
            item = create_prec_item(P_term, new_node);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_stack_pop(stack);
            prec_push_item(stack, item);
            break;
        }
        else expr_wrapper_ThrowError(2);

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
            expr_wrapper_ThrowError(2);
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
        expr_wrapper_ThrowError(2); // cant reduce
        break;
    }

    return;
}
