/** 
* @file expressionparse.h
* @author Patrik Mokrusa (xmokrup00)
*
* IFJ24
*
* @brief Interface precedencniho alanyzatoru
*/

#ifndef EXPRESSIONPARSE_H
#define EXPRESSIONPARSE_H

#include "parser.h"

/**
 * @brief typy precedencnich itemu
 */
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

/**
 * @brief item pro stack
 */
struct PrecStackItem
{
    Prectype type;
    Node* node;
    PrecStackItem * next;
};

/**
 * @brief stack precedencnich itemu
 */
typedef struct PrecStack
{
    int count;
    PrecStackItem * top;
} PrecStack;

/**
 * @brief pomocna funkce pro uvolneni pameni pri error exitu
 */
void expr_wrapper_ThrowError(int code);


/**
 * @brief inicializace precedencniho stacku + vlozi na stack $
 */
PrecStack* prec_stack_init();

/**
 * @brief vytvori a prida PrecItem na stack
 * @param stack ukazatel na stack
 * @param type typ pro PrecItem
 * @param data uzel derivacniho stromu odpovidajici typu itemu
 */
void prec_stack_push(PrecStack * stack, Prectype type, Node * data);

/**
 * @brief pomocna fce pro vytvoreni itemu pro PrecStack
 * @param type typ itemu
 * @param data node odpovidajici PrecItemu
 */
PrecStackItem* create_prec_item(Prectype type, Node * data);

/**
 * @brief pushne item na stack
 * @param stack ukazatel na stack
 * @param item item, ktery chceme pushnout do stacku
 */
void prec_push_item(PrecStack * stack, PrecStackItem* item);

/**
 * @brief popne vrchol stacku
 * @param stack ukazatel na stack
 */
void prec_stack_pop(PrecStack * stack);

/**
 * @brief uvolni stack
 * @param stack ukazatel na stack
 */
void prec_stack_free(PrecStack * stack);

/**
 * @brief consumne token a prevede ho na PrecItem, zaroven incrementuje bracket_cnt pokuc consumne zavorku
 * @param token buffer tokenu
 */
PrecStackItem * next_prec_item(TokenBuffer * token, int* bracket_cnt);

/**
 * @brief zredukuje stack podle pravidel:
 *      1) pokud je na vrcholu stacku neterminal, podiva se na nejblizsi terminal a zredukuje tento terminal a 2 neterminaly (vlevo a vpravo od nej) do jednoho neterminalniho itemu
 *      2) pokud je na vrcholu terminal prave zavorky, zredukuje pravou zavorku, neterminal a levou zavorku do jednoho neterminalniho itemu
 *      3) pokud je na vrcholu "ciselny" terminal, zredukuje ho na neterminal
 * @param stack ukazatel na stack, ktery chceme zredukovat
 * @throw 2 pokud nejde stack zredukovat
 */
void reduce(PrecStack * stack);

/**
 * @brief pomocna funkce pro ziskani typu prvniho neterminalu na stacku
 * @param stack ukazatel na stack
 */
int first_terminal_type(PrecStack* stack);

/**
 * @brief fce co zpracovava Precedencni analyzu
 * @param token buffer tokenu
 */
Node * Parse_expression(TokenBuffer* token);

#endif