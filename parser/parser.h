#ifndef PARSER_H 
#define PARSER_H 

#include "../scanner.h"
#include "../symtable.h"
#include "../error/error.h"
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

    Int_N = 37

} NodeType;

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
    // bool need_converting; /**< bandaid fix pro zaznamenani konverze v id (jinak bude potreva refaktorovat vsechno) */
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
    SymbolTable* sym_table;

}TokenBuffer;

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
 * @return ukazatel na vrcholovy uzel celeho parse tree
 */
Node * Parse_start(TokenBuffer* token);

void free_parse_tree(Node* tree);

/**
 * Dale jen "pomocne" private fce
 */
Node * IdNode_new(char *symtable_key); 
Node * StringNode_new(char *string);
Node * FloatNode_new(double num);
Node * IntNode_new(int num);


void sym_push_params(SymbolTable* table, String* func_key, Node* param_node);
int sym_get_type(int type);



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
Node * Parse_rhs(TokenBuffer* token);
Node * Parse_rhs_param(TokenBuffer* token);
Node * Parse_variable_define(TokenBuffer* token);
Node * Parse_variable_assign(TokenBuffer* token);
Node * Parse_func_call(TokenBuffer* token);
Node * Parse_params(TokenBuffer* token);
Node * Parse_params_next(TokenBuffer* token);
Node * Parse_if(TokenBuffer* token);
Node * Parse_while(TokenBuffer* token);
Node * Parse_void_call(TokenBuffer* token);
Node * Parse_return_statement(TokenBuffer* token);







#endif