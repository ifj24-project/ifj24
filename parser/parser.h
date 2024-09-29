#ifndef PARSER_H 
#define PARSER_H 

#include "../scanner.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

typedef int IdValue;
typedef char* StrValue;
typedef double FloatValue;
typedef int IntValue;

typedef union
{
    IdValue id;
    StrValue str;
    FloatValue flt;
    IntValue inteeger;

} Data_value;

struct Node
{
    int type;
    Node *first;
    Node *second;
    Node *third;
    Node *fourth;
    Data_value data;

};

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
    Float_N = 20
    // doplnit EXPRESSION, null not null while/if

} NodeType;

/**
 * @brief buffer pro parsovani (lookahead)
 */
typedef struct TokenBuffer
{
    Token* first;
    Token* second;
    Token* third;
    Token* fourth;
}TokenBuffer;

/**
 * @brief ctor pro buffer na tokeny, ktery se musi passnout parse_start() fci
 * @return ukazatel na onen buffer
 */
TokenBuffer * buffer_ctor();

/**
 * @brief shiftne tokeny v bufferu (prvni zahodi, zbytek posune a na posledni prida novy)
 * @param token ukazatel na buffer
 */
void consume_buffer(TokenBuffer* token, size_t n);

/**
 * @brief vysledna fce celeho parsingu
 * @param token ukazatel na buffer tokenu vytvorim pomoci buffer_ctor()
 * @return ukazatel na celkovy parse tree
 */
Node * Parse_start(TokenBuffer* token);

/**
 * Dale jen "pomocne" private fce
 */

Node * IdNode_new(int *id_in_sym_table); // bude tam pointer na symtable
Node * StringNode_new(char *string);
Node * FloatNode_new(double num);
Node * IntNode_new(int num);



Node * DataTypeNode_new(int type); // predetermined proradi i32 || f64 || u8 || void

Node * NoChildNode_new(int node_type);
Node * OneChildNode_new(int node_type, Node * first);
Node * TwoChildNode_new(int node_type, Node * first, Node * second);
Node * ThreeChildNode_new(int node_type, Node * first, Node * second, Node * third);
Node * FourChildNode_new(int node_type, Node * first, Node * second, Node * third, Node * fourth);

Node * Parse_id(TokenBuffer* token);
Node * Parse_string(TokenBuffer* token);

Node * Parse_prolog(TokenBuffer* token);
Node * Parse_program(TokenBuffer* token);
Node * Parse_datatype(TokenBuffer* token);
Node * Parse_func_define(TokenBuffer* token);
Node * Parse_params_define(TokenBuffer* token);
Node * Parse_params_define_next(TokenBuffer* token);
Node * Parse_func_body(TokenBuffer* token);
Node * Parse_statement(TokenBuffer* token);
Node * Parse_variable_define(TokenBuffer* token);
Node * Parse_variable_assign(TokenBuffer* token);
Node * Parse_func_call(TokenBuffer* token);
Node * Parse_params(TokenBuffer* token);
Node * Parse_params_next(TokenBuffer* token);
Node * Parse_if(TokenBuffer* token);
Node * Parse_while(TokenBuffer* token);
Node * Parse_void_call(TokenBuffer* token);
Node * Parse_return_statement(TokenBuffer* token);



Node * Parse_expression();  //nastudovat jak implementovat precedencni syntaktickou analyzu



#endif