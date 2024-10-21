#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "expressionparse.h"

// #include "../scanner.h"
// #include "../scanner.c"

/**
 * TODO: 
 * provazat se sym-tablem
 * variable used - kdy to urcit?
 * 
 * parse variable define: fucn_call 
 */

TokenBuffer * buffer_ctor(){
    TokenBuffer * x = malloc(sizeof(TokenBuffer));
    if (x == NULL)
    {
        ThrowError(99);
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

void free_parse_tree(Node* tree){
    if (tree == NULL)
    {
        return;
    }

    switch (tree->type)
    {
    case Id_N:
        free_string(tree->data.id);
        break;
    case Str_N:
        free_string(tree->data.str);
    
    default:
        break;
    }

    free_parse_tree(tree->first);
    free_parse_tree(tree->second);
    free_parse_tree(tree->third);
    free_parse_tree(tree->fourth);

    free(tree);
    
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
        return "[";
        break;
    case T_R_Square_B:
        return "]";
        break;
    case T_L_Curly_B:
        return "{";
        break;
    case T_R_Curly_B:
        return "}";
        break;
    case T_L_Round_B:
        return "(";
        break;
    case T_R_Round_B:
        return ")";
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
    case T_Pipe:
        return "T_Pipe";
        break;
    default:
        break;
    }
}

void sym_push_params(SymbolTable* table, String* func_key, Node* param_node){
    if (param_node == NULL)
    {
        return;
    }
    
    FunctionParam param;
    param.name = param_node->first->data.id;
    switch (param_node->second->data.data_type)
    {
    case 1: //i32
        param.type = TYPE_INT;
        break;
    case 2: //f64
        param.type = TYPE_FLOAT;
        break;
    case 3: //u8
        param.type = TYPE_STRING;
        break;
    case 4: //void
        param.type = TYPE_VOID;
        break;
    case 5: //bool
        param.type = TYPE_BOOL;
        break;
    case -1: //odvozeny?
        param.type = TYPE_UNDEFINED;
        break;
    default:
        printf("sym_push_params something went wrong\n");

        break;
    }

    // params_push(table, func_key, param);  

    sym_push_params(table, func_key, param_node->third); // rekurzivne volam

    return;
}

// FunctionParam* sym_get_params(Node* param){
//     if (param == NULL)
//     {
//         return NULL;
//     }
    
// }
// FunctionParam* sym_new_param(String* name, VarType type, FunctionParam* next){
//     FunctionParam* new_p = malloc(sizeof(FunctionParam));
//     if (new_p == NULL)
//     {
//         ThrowError(99);
//     }
//     new_p->name = name;
//     new_p->type = type;
//     new_p->next = next;

//     return new_p;
// }

void consume_buffer(TokenBuffer* token, size_t n){
    for (size_t i = 0; i < n; i++)
    {
        // printf("token: %s\n",get_token_name(token->first->type));
        free(token->first);
        token->first = token->second;
        token->second = token->third;
        token->third = token->fourth;
        token->fourth = scan();
    }
    
}

void buffer_check_first(TokenBuffer* token, token_type num){
    if (token->first->type != num)
    {
        printf("Expected token type: %s got: %s\n", get_token_name(num), get_token_name(token->first->type));
        printf("next tokens:\n");
        printf("%s\n", get_token_name(token->second->type));
        printf("%s\n", get_token_name(token->third->type));
        printf("%s\n", get_token_name(token->fourth->type));
        ThrowError(2);
    }
    consume_buffer(token, 1);
    return;
}

Node * IdNode_new(char* id_string){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        ThrowError(99);
    }
    x->type = Id_N;
    x->data.id = create_string(id_string); 
    return x;
} 

Node * StringNode_new(char *string){
        Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        ThrowError(99);
    }
    x->type = Id_N;
    x->data.str = create_string(string);
    return x;
}
Node * FloatNode_new(double num){
        Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        ThrowError(99);
    }
    x->type = Id_N;
    x->data.flt = num;
    return x;
}
Node * IntNode_new(int num){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        ThrowError(99);
    }
    x->type = Id_N;
    x->data.integer = num;
    return x;
}

Node * DataTypeNode_new(int type){ // 1==i32, 2==f64, 3==[]u8, 4==void
    Node * x = NoChildNode_new(DataType_N);
    x->data.data_type = type;
    return x;
}

Node * NoChildNode_new(int node_type){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        ThrowError(99);
    }
    x->type = node_type;
    x->first = NULL;
    x->second = NULL;
    x->third = NULL;
    x->fourth = NULL;
    return x;
}
Node * OneChildNode_new(int node_type, Node * first){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        ThrowError(99);
    }
    x->type = node_type;
    x->first = first;
    x->second = NULL;
    x->third = NULL;
    x->fourth = NULL;
    return x;
}
Node * TwoChildNode_new(int node_type, Node * first, Node * second){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        ThrowError(99);
    }
    x->type = node_type;
    x->first = first;
    x->second = second;
    x->third = NULL;
    x->fourth = NULL;
    return x;
}
Node * ThreeChildNode_new(int node_type, Node * first, Node * second, Node * third){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        ThrowError(99);
    }
    x->type = node_type;
    x->first = first;
    x->second = second;
    x->third = third;
    x->fourth = NULL;
    return x;
}
Node * FourChildNode_new(int node_type, Node * first, Node * second, Node * third, Node * fourth){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        ThrowError(99);
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
    if (token->first->type == T_ID)
    {
        char* id_string = token->first->value.ID_name;
        consume_buffer(token, 1);
        return IdNode_new(id_string);
    }
    else
    {
        buffer_check_first(token, T_ID);
    }
    
}
Node * Parse_string(TokenBuffer* token){
    if (token->first->type == T_String)
    {
        char* string = token->first->value.stringVal;
        consume_buffer(token, 1);
        return StringNode_new(string);
    }
    else
    {
        buffer_check_first(token, T_String);
    }
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

    /**
     * TODO: prefill sym table with ifj.functions
     */

    return NoChildNode_new(ProgramProlog_N);
}
Node * Parse_program(TokenBuffer* token){
    if (token->first->type == T_EOF)
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
        printf("Expected data_type_token, got: %s\n",get_token_name(token->first->type));
        ThrowError(2);
        break;
    }

    Node * ret = NoChildNode_new(DataType_N);
    ret->data.data_type=x;
    return ret;
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

    // insert_symbol(token->sym_table, a->data.id, c->data.data_type, create_sym_val(NULL, true, true, false, c->data.data_type));
    

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
        printf("expected statement_token got: %s \n",get_token_name(token->first->type));
        ThrowError(2);
        break;
    }

    return OneChildNode_new(Statement_N, a);
}

Node * Parse_variable_define(TokenBuffer* token){
    Node * a;
    Node * b;
    Node * c;
    int var_const = -1;
    if (token->first->type == T_const)
    {
        consume_buffer(token, 1);
        var_const = 1;
    }
    else if (token->first->type == T_var)
    {
        consume_buffer(token, 1);
        var_const = 0;
    }
    else{
        ThrowError(2);
    }
    
    a = Parse_id(token);

    /** odvozeny typ z fce */
    if (token->first->type == T_Assign)
    {  
        /**
         * TODO: vyhledat fci v sym-table a odvodit typ
         */
        b = NoChildNode_new(DataType_N);
        b->data.data_type = -1; //zjistit datatype z sym-table
    }
    else {
        buffer_check_first(token, T_Colon);
        b = Parse_datatype(token);
    }

    buffer_check_first(token, T_Assign);

    if (token->first->type == T_ID && token->second->type == T_Dot)
    {
        c = Parse_func_call(token);
    }
    else if (token->first->type == T_ID && token->second->type == T_L_Round_B)
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

    

    // insert_symbol(token->sym_table, a->data.id, b->data.data_type, create_sym_val(var_const, false, true, false, a->data.data_type));

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
    bool is_ifj = false;
    if (token->second->type == T_Dot)
    {
        buffer_check_first(token, T_ID); //check if ifj.func
        buffer_check_first(token, T_Dot);
        is_ifj = true;
    }
    
    Node * a = Parse_id(token); 
    buffer_check_first(token, T_L_Round_B);
    Node * b = Parse_params(token);
    buffer_check_first(token, T_R_Round_B);

    if (is_ifj)
    {
        String* ifj = create_string("ifj.");
        a->data.id = concat_strings(ifj, a->data.id);
        free_string(ifj);
    }
    

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
    
    if (token->first->type == T_Pipe)
    {
        consume_buffer(token, 1);
        Node * b = Parse_id(token);
        buffer_check_first(token, T_Pipe);

        buffer_check_first(token, T_L_Curly_B);
        Node * c = Parse_func_body(token);
        buffer_check_first(token, T_R_Curly_B);

        // TODO: else not mandatory?
        buffer_check_first(token, T_else);
        buffer_check_first(token, T_L_Curly_B);
        Node * d = Parse_func_body(token);
        buffer_check_first(token, T_R_Curly_B);

        Node * ret = FourChildNode_new(If_N, a, b, c, d);
        ret->data.has_not_null_id = true;
        return ret;

    }
    

    buffer_check_first(token, T_L_Curly_B);
    Node * b = Parse_func_body(token);
    buffer_check_first(token, T_R_Curly_B);
    // TODO: else not mandatory?
    buffer_check_first(token, T_else);
    buffer_check_first(token, T_L_Curly_B);
    Node * c = Parse_func_body(token);
    buffer_check_first(token, T_R_Curly_B);

    return ThreeChildNode_new(If_N, a, b, c);
}

Node * Parse_while(TokenBuffer* token){
    buffer_check_first(token, T_while);
    buffer_check_first(token, T_L_Round_B);
    Node * a = Parse_expression(token);
    buffer_check_first(token, T_R_Round_B);
    
    if (token->first->type == T_Pipe)
    {
        consume_buffer(token, 1);
        Node * b = Parse_id(token);
        buffer_check_first(token, T_Pipe);
        buffer_check_first(token, T_L_Curly_B);
        Node * c = Parse_func_body(token);
        buffer_check_first(token, T_R_Curly_B);

        

        Node * ret = ThreeChildNode_new(While_N, a, b, c);
        ret->data.has_not_null_id = true;
        return ret;

    }
    

    buffer_check_first(token, T_L_Curly_B);
    Node * b = Parse_func_body(token);
    buffer_check_first(token, T_R_Curly_B);

    return TwoChildNode_new(While_N, a, b);
}

Node * Parse_void_call(TokenBuffer* token){
    bool is_ifj = false;

    if (token->second->type == T_Dot)
    {   
        buffer_check_first(token, T_ID); // check if ifj
        buffer_check_first(token, T_Dot);
        is_ifj = true;
    }
    
    Node * a = Parse_id(token);

    buffer_check_first(token, T_L_Round_B);
    Node * b = Parse_params(token);
    buffer_check_first(token, T_R_Round_B);
    buffer_check_first(token, T_SemiC);

    if (is_ifj)
    {
        String* ifj = create_string("ifj.");
        a->data.id = concat_strings(ifj, a->data.id);
        free_string(ifj);
    }
    
    
    return TwoChildNode_new(VoidCall_N, a, b);
}

Node * Parse_return_statement(TokenBuffer* token){
    buffer_check_first(token, T_return);
    Node * a = Parse_expression(token);
    buffer_check_first(token, T_SemiC);

    return OneChildNode_new(ReturnStatement_N, a);
}

