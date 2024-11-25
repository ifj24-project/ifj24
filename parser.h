/** 
* @file parser.h
* @author Patrik Mokrusa (xmokrup00)
*
* IFJ24
*
* @brief Interface rekurzivniho sestupu
*/

#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Node Node;

/**
 * @brief typy uzlu - pocet deti vychazi z ll-gramatiky
 */
typedef enum NodeType {
    Start_N = 0,
    ProgramProlog_N = 1,
    Program_N = 2,
    DataType_N = 3,
    FuncDefine_N = 4,
    ParamsDefine_N = 5,
    ParamsDefineNext_N = 6,
    FuncBody_N = 7,
    Statement_N = 8,
    VariableDefine_N = 9,
    VariableAssign_N = 10,
    FuncCall_N = 11,
    Params_N = 12,
    ParamsNext_N = 13,
    If_N = 14,
    While_N = 15,
    VoidCall_N = 16,
    ReturnStatement_N = 17,
    Id_N = 18,
    Str_N = 19,
    Float_N = 20,
    If_not_null = 21,
    while_not_null =22,
    // EXPRESSION
    Expression_N = 23,
    Compared_N = 24,
    Term_N = 25,
    Factor_N = 26,
    Lesser_N = 27,
    LesserEq_N = 28,
    Greater_N = 29,
    GreaterEq_N = 30,
    Eq_N = 31,
    NotEq_N = 32,
    Plus_N = 33,
    Minus_N = 34,
    Times_N = 35,
    Divide_N = 36,

    Int_N = 37,
    Underscore_N = 38

} NodeType;

/**
 * @brief Datove typy
 */
typedef enum DataTypeEnum{
    DT_UNDEFINED = -1,
    DT_I32 = 1,
    DT_F64 = 2,
    DT_U8 = 3,
    DT_VOID = 4,
    DT_BOOL = 5,
    DT_I32_NULL = 6,
    DT_F64_NULL = 7,
    DT_U8_NULL = 8,
    DT_BOOL_NULL = 9

} DataTypeEnum;

typedef String* IdValue;
typedef String* StrValue;
typedef double FloatValue;
typedef int IntValue;
typedef int DataTypeValue;
typedef int VariableDefineValue;
typedef bool Not_null_statement;
typedef SymbolTable* FuncDefineValue;
typedef struct
{
    VarType left;
    VarType right;

} BoolValue;

/**
 * @brief specialni members pro nektere uzly
 */
typedef union DataValue
{
    IdValue id; // value pro id
    StrValue str; // value pro string node
    FloatValue flt; // value pro float node
    IntValue integer; // value pro int node
    DataTypeValue data_type; // 1==i32, 2==f64, 3==[]u8, 4==void , 5==bool
    VariableDefineValue var_or_const; // 0==var, 1==const
    Not_null_statement has_not_null_id; // true== ma o dite navic, node.second dite je |id| v te pipe
    FuncDefineValue sym_table; // Dane: musis znova pushnout var v tech |pipech| protoze ja je popuju po jejich useku platnosti;
    BoolValue bool_val; // udava vartype na leve a prave stane u relacnich operatoru

} Data_value;

/**
 * @brief struktura uzlu parse stromu
 */
struct Node
{
    NodeType type; /**< typ uzlu */
    Node *first; /**< ukazatal na prvni uzel (prvni dite) */
    Node *second; /**< ukazatal na druhe uzel (druhe dite) */
    Node *third; /**< ukazatal na treti uzel (treti dite) */
    Node *fourth; /**< ukazatal na ctvrty uzel (ctvrte dite) */
    Data_value data; /**< nektere node typy maji specialni value. Node muze mit pouze jedny data */
};

/**
 * @brief buffer pro parsovani (lookahead)
 */
typedef struct TokenBuffer
{
    Token* first;
    Token* second;
    Token* third;
    Token* fourth;
    SymbolTable* sym_table; // je tady, protoze to byla cesta nejmensiho odporu

}TokenBuffer;


/**
 * @brief item pro stack nodu
 */
typedef struct NodeStackItem
{
    Node* data;
    struct NodeStackItem * next;

} NodeStackItem;

/**
 * @brief stack pro nody, existuje z duvodu uvolnovani pameti pri erroru
 */
typedef struct NodeStack
{
    int count;
    NodeStackItem* first;

} NodeStack;

/**
 * @brief zajistuje uvolneni pri error exitu
 */
void parse_wrapper_ThrowError(int code);

/**
 * @brief ctor pro buffer na tokeny, ktery se musi passnout parse_start() fci
 * @return ukazatel na onen buffer
 */
TokenBuffer * buffer_ctor();

/**
 * @brief destructor pro buffer na tokeny
 * @param[in] token bere ukazatel na buffer, ktery chceme dat free()
 */
void buffer_dtor(TokenBuffer * token);
/**
 * @brief shiftne tokeny v bufferu (prvni zahodi, zbytek posune a na posledni prida novy)
 * @param[in] token ukazatel na buffer
 * @param[in] n kolik tokenu chceme shiftnout
 */
void consume_buffer(TokenBuffer* token, size_t n);

/**
 * @brief vysledna fce celeho parsingu
 * @param[in] token ukazatel na buffer tokenu vytvorim pomoci buffer_ctor()
 * @return ukazatel na koren parse tree
 */
Node * Parse_start(TokenBuffer* token);


/**
 * @brief uvolni parse tree
 * @param[in] tree ukazatel na korenovy uzel stromu
 */
void free_parse_tree(Node* tree);






/**
 * Dale jen "pomocne" private fce
 */





/**
 * @brief basic stack funkce
 */
NodeStack* node_stack_init();
void node_stack_push(NodeStack* stack, Node* node);
void node_stack_free(NodeStack* stack);
void node_stack_free_keep_nodes(NodeStack* stack);

/**
 * @brief pomocna funkce pro vytvoreni uzlu typu id_N
 * @param[in] symtable_key jmeno id
 */
Node * IdNode_new(char *symtable_key);

/**
 * @brief pomocna funkce pro vytvoreni uzlu typu str_N
 * @param[in] string hodnota string
 */
Node * StringNode_new(char *string);

/**
 * @brief pomocna funkce pro vytvoreni uzlu typu flt_N
 * @param[in] num hodnota float
 */
Node * FloatNode_new(double num);

/**
 * @brief pomocna funkce pro vytvoreni uzlu typu int_N
 * @param[in] num hodnota int
 */
Node * IntNode_new(int num);

/**
 * @brief pomocna funkce pro vytvoreni uzlu typu datatype_N
 * @param[in] type typ z DataTypeEnum
 */
Node * DataTypeNode_new(int type); 


/**
 * @brief pomocna funkce pro pushovani parametru do symtablu
 * @param table symtab
 * @param func_key klic
 * @param[in] param_node uzel typu param
 */
void sym_push_params(SymbolTable* table, String* func_key, Node* param_node);

/**
 * @brief pomocna funkce pro konverzi enumu (parser enum -> symtab enum)
 * @param[in] type 
 * @return zkonvertovany typ
 */
int sym_get_type(int type);

/**
 * @brief pomocna funkce pro vytvoreni uzlu (ukazatele na deti vyplni NULL)
 * @param[in] node_type typ uzlu
 */
Node * NoChildNode_new(int node_type);

/**
 * @brief pomocna funkce pro vytvoreni uzlu (tri posledni ukazatele vyplni NULL)
 * @param[in] node_type typ uzlu
 * @param first ukazatel na prvni dite uzlu
 * @return ukazatel na uzel typu node_type
 */
Node * OneChildNode_new(int node_type, Node * first);

/**
 * @brief pomocna funkce pro vytvoreni uzlu (2 posledni ukazatele vyplni NULL)
 * @param[in] node_type typ uzlu
 * @param first ukazatel na prvni dite uzlu
 * @param second ukazatel na druhe dite uzlu
 * @return ukazatel na uzel typu node_type
 */
Node * TwoChildNode_new(int node_type, Node * first, Node * second);

/**
 * @brief pomocna funkce pro vytvoreni uzlu (posledni ukazatele vyplni NULL)
 * @param[in] node_type typ uzlu
 * @param first ukazatel na prvni dite uzlu
 * @param second ukazatel na druhe dite uzlu
 * @param third ukazatel na treti dite uzlu
 * @return ukazatel na uzel typu node_type
 */
Node * ThreeChildNode_new(int node_type, Node * first, Node * second, Node * third);

/**
 * @brief pomocna funkce pro vytvoreni uzlu
 * @param[in] node_type typ uzlu
 * @param first ukazatel na prvni dite uzlu
 * @param second ukazatel na druhe dite uzlu
 * @param third ukazatel na treti dite uzlu
 * @param fourth ukazatel ne ctvrte dite uzlu
 * @return ukazatel na uzel typu node_type
 */
Node * FourChildNode_new(int node_type, Node * first, Node * second, Node * third, Node * fourth);

/**
 * @brief zpracovava pravildo 5 ll-gramatiky
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_id(TokenBuffer* token);

/**
 * @brief zpracovava pravildo 8 ll-gramatiky
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_string(TokenBuffer* token);

/**
 * @brief zpracovava pravildo 2 ll-gramatiky
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_prolog(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 3, 4
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_program(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 9, 10, 11, 12
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_datatype(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 13
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_func_define(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 14, 15
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_params_define(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 16, 17
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_params_define_next(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 18, 19
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_func_body(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 20, 21, 22, 23, 24, 25
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_statement(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 28, 29
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_rhs(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 26, 27
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_rhs_param(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 30, 31
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_variable_define(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 32, 33
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_variable_assign(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 34
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_func_call(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 35, 36
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_params(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 37, 38
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_params_next(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 39, 40
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_if(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 41, 42
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_while(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 43
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_void_call(TokenBuffer* token);

/**
 * @brief zpracovava pravilda ll-gramatiky: 44, 45
 * @param token buffer tokenu (pass parameter)
 */
Node * Parse_return_statement(TokenBuffer* token);

#endif
