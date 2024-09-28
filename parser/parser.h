#ifndef PARSER.H
#define PARSER.H

typedef struct Node
{
    NodeType type;
    Node *first = nullptr;
    Node *second = nullptr;
    Node *third = nullptr;
    Node *fourth = nullptr;

};


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
    ReturnStatement_N = 17
    // doplnit EXPRESSION, null not null while/if

} NodeType;

Node * IdNode_new(int *id_in_sym_table); // bude tam pointer na symtable
Node * DataTypeNode_new(int type); // predetermined proradi i32 || f64 || u8 || void

Node * OneChildNode_new(int node_type, Node * first);
Node * TwoChildNode_new(int node_type, Node * first, Node * second);
Node * ThreeChildNode_new(int node_type, Node * first, Node * second, Node * third);
Node * FourChildNode_new(int node_type, Node * first, Node * second, Node * third, Node * fourth);

#endif