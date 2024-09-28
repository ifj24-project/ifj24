#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"

Node * IdNode_new(int *id_in_sym_table){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(2);
    }
    x->type = Id_N;
    // x->data.id = id_in_sym_table; 
    return x;
} // bude tam pointer na symtable

Node * StringNode_new(char *string){
        Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(2);
    }
    x->type = Id_N;
    x->data.str = string;
    return x;
}
Node * FloatNode_new(float num){
        Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(2);
    }
    x->type = Id_N;
    x->data.flt = num;
    return x;
}
Node * IntNode_new(int num){
        Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(2);
    }
    x->type = Id_N;
    x->data.inteeger = num;
    return x;
}

Node * DataTypeNode_new(int type); // predetermined proradi i32 || f64 || u8 || void

Node * OneChildNode_new(int node_type, Node * first){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(2);
    }
    x->type = node_type;
    x->first = first;
    return x;
}
Node * TwoChildNode_new(int node_type, Node * first, Node * second){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(2);
    }
    x->type = node_type;
    x->first = first;
    x->second = second;
    return x;
}
Node * ThreeChildNode_new(int node_type, Node * first, Node * second, Node * third){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(2);
    }
    x->type = node_type;
    x->first = first;
    x->second = second;
    x->third = third;
    return x;
}
Node * FourChildNode_new(int node_type, Node * first, Node * second, Node * third, Node * fourth){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(2);
    }
    x->type = node_type;
    x->first = first;
    x->second = second;
    x->third = third;
    x->fourth = fourth;
    return x;
}

Node * Parse_start(){
    Node * first = Parse_prolog();
    Node * second = Parse_program();

    return TwoChildNode_new(Start_N,first, second);
}

Node * Parse_id(){}
Node * Parse_string(){}

Node * Parse_prolog(){}
Node * Parse_program(){}
Node * Parse_datatype(){}
Node * Parse_func_define(){}
Node * Parse_params_define(){}
Node * Parse_params_define_next(){}
Node * Parse_func_body(){}
Node * Parse_statement(){}
Node * Parse_variable_define(){}
Node * Parse_variable_assign(){}
Node * Parse_func_call(){}
Node * Parse_params(){}
Node * Parse_params_next(){}
Node * Parse_if(){}
Node * Parse_while(){}
Node * Parse_void_call(){}
Node * Parse_return_statement(){}