#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
// #include "../scanner.h"
// #include "../scanner.c"

/**
 * TODO: 
 * kontrola prologu
 * provazat se scannerem
 * consume
 * id node new / parse id
 * string node new / parse string
 * parse variable define: fucn_call 
 */



TokenBuffer * buffer_ctor(){
    TokenBuffer * x = malloc(sizeof(TokenBuffer));
    if (x == NULL)
    {
        exit(99);
    }
    x->first = scan();
    x->second = scan();
    x->third = scan();
    x->fourth = scan();

    return x;
}

void buffer_dtor(TokenBuffer * token){
    free(token->first);
    free(token->second);
    free(token->third);
    free(token->fourth);
    free(token);

    return;
}

const char * get_token_name(enum token_type token_id){
    switch (token_id)
    {
    case T_Minus:
        return "T_Minus";
        break;
    case T_Plus:
        return "T_Plus";
        break;
    case T_Mul:
        return "T_Mul";
        break;
    case T_Div:
        return "T_Div";
        break;
    case T_Assign:
        return "T_Assign";
        break;
    case T_Equal:
        return "T_Equal";
        break;
    case T_Lesser:
        return "T_Lesser";
        break;
    case T_Less_Eq:
        return "T_Less_Eq";
        break;
    case T_Greater:
        return "T_Greater";
        break;
    case T_Great_Eq:
        return "T_Great_Eq";
        break;
    case T_Not_Eq:
        return "T_Not_Eq";
        break;
    case T_L_Square_B:
        return "T_L_Square_B";
        break;
    case T_R_Square_B:
        return "T_R_Square_B";
        break;
    case T_L_Curly_B:
        return "T_L_Curly_B";
        break;
    case T_R_Curly_B:
        return "T_R_Curly_B";
        break;
    case T_L_Round_B:
        return "T_L_Round_B";
        break;
    case T_R_Round_B:
        return "T_R_Round_B";
        break;
    case T_Percent:
        return "%";
        break;
    case T_SemiC:
        return ";";
        break;
    case T_Comma:
        return ",";
        break;
    case T_Dot:
        return "T_Dot";
        break;
    case T_At:
        return "@";
        break;
    case T_Colon:
        return ":";
        break;
    case T_Question:
        return "?";
        break;
    case T_Underscore:
        return "_";
        break;
    case T_const:
        return "T_const";
        break;
    case T_else:
        return "T_else";
        break;
    case T_fn:
        return "T_fn";
        break;
    case T_if:
        return "T_if";
        break;
    case T_i32:
        return "T_i32";
        break;
    case T_f64:
        return "T_f64";
        break;
    case T_null:
        return "T_null";
        break;
    case T_pub:
        return "T_pub";
        break;
    case T_return:
        return "T_return";
        break;
    case T_u8:
        return "T_u8";
        break;
    case T_var:
        return "T_var";
        break;
    case T_void:
        return "T_void";
        break;
    case T_while:
        return "T_while";
        break;
    case T_import:
        return "T_import";
        break;
    case T_ID:
        return "T_ID";
        break;
    case T_Integer:
        return "T_Integer";
        break;
    case T_Float:
        return "T_Float";
        break;
    case T_Exponent:
        return "T_Exponent";
        break;
    case T_String:
        return "T_String";
        break;
    case T_EOF:
        return "T_EOF";
        break;
    case T_ERORR:
        return "T_ERROR";
        break;
    default:
        break;
    }
}

void consume_buffer(TokenBuffer* token, size_t n){
    for (size_t i = 0; i < n; i++)
    {
        printf("token: %s\n",get_token_name(token->first->type));
        free(token->first);
        token->first = token->second;
        token->second = token->third;
        token->third = token->fourth;
        token->fourth = scan();
        //TODO: free vsechno, kdyz EOF
    }
    
}

void buffer_check_first(TokenBuffer* token, token_type num){
    if (token->first->type != num)
    {
        printf("Expected token type: %s ; got: %s\n", get_token_name(num), get_token_name(token->first->type));
        exit(2);
    }
    consume_buffer(token, 1);
    return;
}

Node * IdNode_new(int *id_in_sym_table){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(99);
    }
    x->type = Id_N;
    // x->data.id = id_in_sym_table; 
    return x;
} // bude tam pointer na symtable

Node * StringNode_new(char *string){
        Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(99);
    }
    x->type = Id_N;
    x->data.str = string;
    return x;
}
Node * FloatNode_new(double num){
        Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(99);
    }
    x->type = Id_N;
    x->data.flt = num;
    return x;
}
Node * IntNode_new(int num){
        Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(99);
    }
    x->type = Id_N;
    x->data.inteeger = num;
    return x;
}

Node * DataTypeNode_new(int type){ // predetermined proradi i32 || f64 || u8 || void
    Node * x = NoChildNode_new(DataType_N);
    x->data.data_type = type;
    return x;
}

Node * NoChildNode_new(int node_type){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(99);
    }
    x->type = node_type;
    return x;
}
Node * OneChildNode_new(int node_type, Node * first){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(99);
    }
    x->type = node_type;
    x->first = first;
    return x;
}
Node * TwoChildNode_new(int node_type, Node * first, Node * second){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        exit(99);
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
        exit(99);
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
        exit(99);
    }
    x->type = node_type;
    x->first = first;
    x->second = second;
    x->third = third;
    x->fourth = fourth;
    return x;
}

Node * Parse_start(TokenBuffer* token){
    Node * first = Parse_prolog(token);
    Node * second = Parse_program(token);

    return TwoChildNode_new(Start_N,first, second);
}

Node * Parse_id(TokenBuffer* token){
    /**
     * placeholder
     */
    buffer_check_first(token, T_ID);
    return NULL;
}
Node * Parse_string(TokenBuffer* token){
    /**
     * placeholder
     */
    buffer_check_first(token, T_String);
    return NULL;
}

Node * Parse_prolog(TokenBuffer* token){
    buffer_check_first(token, T_const);
    buffer_check_first(token, T_ID);
    buffer_check_first(token, T_Assign);
    buffer_check_first(token, T_At);
    buffer_check_first(token, T_import);
    buffer_check_first(token, T_L_Round_B);
    buffer_check_first(token, T_String);
    buffer_check_first(token, T_R_Round_B);
    buffer_check_first(token, T_SemiC);

    return NoChildNode_new(ProgramProlog_N);
}
Node * Parse_program(TokenBuffer* token){
    if (token->first->type == T_EOF) //prispusobit
    {
        return NULL;
    }

    Node* x = Parse_func_define(token);
    Node* y = Parse_program(token);

    return TwoChildNode_new(Program_N, x, y);
}

Node * Parse_datatype(TokenBuffer* token){
    int x;

    switch (token->first->type)
    {
    case T_i32:
        consume_buffer(token,1);
        x = 1;
        break;
    
    case T_f64:
        consume_buffer(token, 1);
        x = 2;
        break;
        
    case T_L_Square_B:
        buffer_check_first(token, T_L_Square_B);
        buffer_check_first(token,T_R_Square_B);
        buffer_check_first(token, T_u8);
        x = 3;
        break;
    
    case T_void:
        consume_buffer(token, 1);
        x = 4;
        break;

    default:
        printf("Expected data_token, got: %s\n",get_token_name(token->first->type));
        exit(2);
        break;
    }
    
}

Node * Parse_func_define(TokenBuffer* token){
    buffer_check_first(token, T_pub);
    buffer_check_first(token, T_fn);

    Node * a = Parse_id(token);
    buffer_check_first(token, T_L_Round_B);
    Node * b = Parse_params_define(token);
    buffer_check_first(token, T_R_Round_B);
    Node * c = Parse_datatype(token);
    buffer_check_first(token, T_L_Curly_B);
    Node * d = Parse_func_body(token);
    buffer_check_first(token, T_R_Curly_B);

    return FourChildNode_new(FuncDefine_N, a, b, c, d);
}

Node * Parse_params_define(TokenBuffer* token){
    if (token->first->type == T_R_Round_B)
    {
        return NULL;
    }

    Node * a = Parse_id(token);
    buffer_check_first(token, T_Colon);
    Node * b = Parse_datatype(token);
    Node * c = Parse_params_define_next(token);

    return ThreeChildNode_new(ParamsDefine_N, a, b, c);
}

Node * Parse_params_define_next(TokenBuffer* token){
    if (token->first->type == T_R_Round_B)
    {
        return NULL;
    }

    buffer_check_first(token, T_Comma);

    Node * a = Parse_id(token);
    buffer_check_first(token, T_Colon);
    Node * b = Parse_datatype(token);
    Node * c = Parse_params_define_next(token);

    return ThreeChildNode_new(ParamsDefineNext_N, a, b, c);
}

Node * Parse_func_body(TokenBuffer* token){
    if (token->first->type == T_R_Curly_B){return NULL;}

    Node * a = Parse_statement(token);
    Node * b = Parse_func_body(token);

    return TwoChildNode_new(FuncBody_N, a, b);
}

Node * Parse_statement(TokenBuffer* token){
    Node * a;

    switch (token->first->type)
    {
    case T_const:
        a = Parse_variable_define(token);
        break;
    case T_var:
        a = Parse_variable_define(token);
        break;
    case T_ID: // var assign or void call
        if ((token->second->type == T_L_Round_B)||(token->second->type == T_Dot))
        {
            a = Parse_void_call(token);
        }
        else {
            a = Parse_variable_assign(token);
        }
        
        break;
    case T_if:
        a = Parse_if(token);
        break;
    case T_while:
        a = Parse_while(token);
        break;
    case T_return:
        a = Parse_return_statement(token);
        break;
    default:
        printf("expected statement got token type: %s \n",get_token_name(token->first->type));
        exit(2);
        break;
    }

    return OneChildNode_new(Statement_N, a);
}

Node * Parse_variable_define(TokenBuffer* token){
    Node * a;
    Node * b;
    Node * c;
    int var_const = -1;
    // TODO: narvat to do sym-tablu az bude
    if (token->first->type == T_const)
    {
        consume_buffer(token, 1);
        var_const = 1;
        /* code */
    }
    else if (token->first->type == T_var)
    {
        consume_buffer(token, 1);
        var_const = 0;
        /* code */
    }
    else{
        exit(2);
    }
    
    a = Parse_id(token);
    buffer_check_first(token, T_Colon);
    b = Parse_datatype(token);
    buffer_check_first(token, T_Assign);

    if (token->first->type == T_ID && token->second->type == T_Dot) //TODO: && id is function
    {
        c = Parse_func_call(token);
    }
    else if (token->first->type == T_String)
    {
        c = Parse_string(token);
    }
    else {
        c = Parse_expression(token);
    }

    buffer_check_first(token, T_SemiC);

    Node * ret = ThreeChildNode_new(VariableDefine_N, a, b, c);
    ret->data.var_or_const = var_const;
    return ret;
}

Node * Parse_variable_assign(TokenBuffer* token){
    Node * a = Parse_id(token);
    buffer_check_first(token, T_Assign);
    Node * b;
    if (token->first->type == T_ID && token->second->type == T_Dot) //TODO: && id is function
    {
        b = Parse_func_call(token);
    }
    else if (token->first->type == T_String)
    {
        b = Parse_string(token);
    }
    else {
        b = Parse_expression(token);
    }

    buffer_check_first(token, T_SemiC);
    return TwoChildNode_new(VariableAssign_N, a, b);
}

Node * Parse_func_call(TokenBuffer* token){
    Node * a = Parse_id(token);
    buffer_check_first(token, T_L_Round_B);
    Node * b = Parse_params(token);
    buffer_check_first(token, T_R_Round_B);

    return TwoChildNode_new(FuncCall_N, a, b);
}

Node * Parse_params(TokenBuffer* token){
    if (token->first->type == T_R_Round_B)
    {
        return NULL;
    }

    Node * a;

    if (token->first->type == T_String)
    {
        a = Parse_string(token);
    }
    else if ((token->first->type == T_ID) && (token->second->type == T_Comma))
    {
        a = Parse_id(token);
    }
    else if ((token->first->type == T_ID) && (token->second->type == T_R_Round_B))
    {
        a = Parse_id(token);
    }
    else {
        a = Parse_expression(token);
    }
    
    Node * b = Parse_params_next(token);

    return TwoChildNode_new(Params_N, a, b);
}

Node * Parse_params_next(TokenBuffer* token){
        if (token->first->type == T_R_Round_B)
    {
        return NULL;
    }

    buffer_check_first(token, T_Comma);
    Node * a;

    if (token->first->type == T_String)
    {
        a = Parse_string(token);
    }
    else if ((token->first->type == T_ID) && (token->second->type == T_Comma))
    {
        a = Parse_id(token);
    }
    else if ((token->first->type == T_ID) && (token->second->type == T_R_Round_B))
    {
        a = Parse_id(token);
    }
    else {
        a = Parse_expression(token);
    }
    
    Node * b = Parse_params_next(token);

    return TwoChildNode_new(ParamsNext_N, a, b);
}

Node * Parse_if(TokenBuffer* token){
    buffer_check_first(token, T_if);
    buffer_check_first(token, T_L_Round_B);
    Node * a = Parse_expression(token);
    buffer_check_first(token, T_R_Round_B);
    /**
     * TODO: check for pipe
     */

    buffer_check_first(token, T_L_Curly_B);
    Node * b = Parse_func_body(token);
    buffer_check_first(token, T_R_Curly_B);
    buffer_check_first(token, T_else);
    buffer_check_first(token, T_L_Curly_B);
    Node * c = Parse_func_body(token);
    buffer_check_first(token, T_R_Curly_B);

    return ThreeChildNode_new(If_N, a, b, c);
}

// Node * Parse_if_not_null(TokenBuffer* token){
//     buffer_check_first(token, T_if);
//     buffer_check_first(token, T_L_Round_B);
//     Node * a = Parse_expression(token);
//     buffer_check_first(token, T_R_Round_B);
//     /**
//      * TODO: check for pipe
//      */
//     Node * b;


//     buffer_check_first(token, T_L_Curly_B);
//     Node * c = Parse_func_body(token);
//     buffer_check_first(token, T_R_Curly_B);
//     buffer_check_first(token, T_else);
//     buffer_check_first(token, T_L_Curly_B);
//     Node * d = Parse_func_body(token);
//     buffer_check_first(token, T_R_Curly_B);

//     return FourChildNode_new(If_N, a, b, c, d);
// }

Node * Parse_while(TokenBuffer* token){
    buffer_check_first(token, T_while);
    buffer_check_first(token, T_L_Round_B);
    Node * a = Parse_expression(token);
    buffer_check_first(token, T_R_Round_B);
    /**
     * TODO: check for pipe
     */
    

    buffer_check_first(token, T_L_Curly_B);
    Node * b = Parse_func_body(token);

    return TwoChildNode_new(While_N, a, b);
}

// Node * Parse_while_not_null(TokenBuffer* token){
//     buffer_check_first(token, T_while);
//     buffer_check_first(token, T_L_Round_B);
//     Node * a = Parse_expression(token);
//     buffer_check_first(token, T_R_Round_B);
//     /**
//      * TODO: check for pipe
//      */
//     Node * b;


//     buffer_check_first(token, T_L_Curly_B);
//     Node * c = Parse_func_body(token);

//     return ChildNode_new(While_N, a, b, c);
// }

Node * Parse_void_call(TokenBuffer* token){
    Node * a = Parse_id(token);

    if (token->first->type == T_Dot)
    {
        buffer_check_first(token, T_Dot);
        buffer_check_first(token, T_ID);
    }
    

    buffer_check_first(token, T_L_Round_B);
    Node * b = Parse_params(token);
    buffer_check_first(token, T_R_Round_B);
    buffer_check_first(token, T_SemiC);
    
    return TwoChildNode_new(VoidCall_N, a, b);
}

Node * Parse_return_statement(TokenBuffer* token){
    buffer_check_first(token, T_return);
    Node * a = Parse_expression(token);
    buffer_check_first(token, T_SemiC);

    return OneChildNode_new(ReturnStatement_N, a);
}

Node * Parse_expression(TokenBuffer* token){
    /**
     * placeholder
     */
    return NULL;
}