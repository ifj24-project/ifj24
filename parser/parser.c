#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../error/error.h"
#include "parser.h"
#include "expressionparse.h"

/**
 * ERROR HANDLING
 */

TokenBuffer * PARSER_TOKEN_BUFFER = NULL;
NodeStack * PARSER_NODE_STACK = NULL;

void parse_wrapper_ThrowError(int code){

    if (PARSER_NODE_STACK != NULL) node_stack_free(PARSER_NODE_STACK);
    
    if (PARSER_TOKEN_BUFFER != NULL) {
        free_symbol_table(PARSER_TOKEN_BUFFER->sym_table);
        buffer_dtor(PARSER_TOKEN_BUFFER);
    }
    ThrowError(code);
}


NodeStack* node_stack_init(){
    NodeStack* stack = (NodeStack*) malloc(sizeof(NodeStack));
    if (stack == NULL) parse_wrapper_ThrowError(99);
    stack->count = 0;
    stack->first = NULL;
    return stack;
}

void node_stack_push(NodeStack* stack, Node* node){
    // stack = PARSER_NODE_STACK;
    NodeStackItem* item = (NodeStackItem*) malloc(sizeof(NodeStackItem));
    if (item == NULL) parse_wrapper_ThrowError(99);

    item->data = node;
    item->next = stack->first;
    stack->first = item;
    stack->count++;
}

void node_stack_free(NodeStack* stack){
    NodeStackItem* item = stack->first;
    // NodeStackItem* item = PARSER_NODE_STACK->first;
    NodeStackItem* temp;
    while (item != NULL)
    {
        temp = item->next;
        switch (item->data->type)
        {
        case Id_N:
            free_string(item->data->data.id);
            break;
        case Str_N:
            free_string(item->data->data.str);
            break;
        default:
            break;
        }
        free(item->data);
        
        free(item);
        item = temp;
    }
    free(stack);
}

void node_stack_free_keep_nodes(NodeStack* stack){
    NodeStackItem* item = stack->first;
    NodeStackItem* temp;
    while (item != NULL)
    {
        temp = item->next;
        free(item);
        item = temp;
    }
    free(stack);
}

TokenBuffer * buffer_ctor(){
    TokenBuffer * x = malloc(sizeof(TokenBuffer));
    if (x == NULL)
    {
        parse_wrapper_ThrowError(99);
    }
    x->first = scan();
    x->second = scan();
    x->third = scan();
    x->fourth = scan();

    if (x->first->type == T_ERORR ||
        x->second->type == T_ERORR ||
        x->third->type == T_ERORR ||
        x->fourth->type == T_ERORR) {
            fprintf(stderr, "\nERROR TOKEN DETECTED!\n\n");
            parse_wrapper_ThrowError(99);
        }

    return x;
}

/**
 * HELPERS & UTILS
 */

void buffer_dtor(TokenBuffer * token){
    if (token->first->type == T_String) free(token->first->value.stringVal);
    if (token->first->type == T_ID) free(token->first->value.ID_name);
    free(token->first);
    if (token->second->type == T_String) free(token->second->value.stringVal);
    if (token->second->type == T_ID) free(token->second->value.ID_name);
    free(token->second);
    if (token->third->type == T_String) free(token->third->value.stringVal);
    if (token->third->type == T_ID) free(token->third->value.ID_name);
    free(token->third);
    if (token->fourth->type == T_String) free(token->fourth->value.stringVal);
    if (token->fourth->type == T_ID) free(token->fourth->value.ID_name);
    free(token->fourth);
    free(token);

    return;
}

void free_parse_tree(Node* tree){
    if (tree == NULL)
    {
        return;
    }

    free_parse_tree(tree->first);
    free_parse_tree(tree->second);
    free_parse_tree(tree->third);
    free_parse_tree(tree->fourth);

    switch (tree->type)
    {
    case Id_N:
        free_string(tree->data.id);
        break;
    case Str_N:
        free_string(tree->data.str);
        break;
    case FuncDefine_N:
        if (tree->data.sym_table != NULL) free_symbol_table(tree->data.sym_table);
        
        break;
    default:
        break;
    }

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
        return "ERROR: TOKEN TYPE NOT FOUND";
        break;
    }
}


int sym_get_type(int type){
        switch (type)
    {
    case DT_I32: //i32
        return TYPE_INT;
        break;
    case DT_F64: //f64
        return TYPE_FLOAT;
        break;
    case DT_U8: //u8
        return TYPE_STRING;
        break;
    case DT_VOID: //void
        return TYPE_VOID;
        break;
    case DT_BOOL: //bool
        return TYPE_BOOL;
        break;
    case DT_UNDEFINED: //odvozeny?
        return TYPE_UNDEFINED;
        break;
    case DT_I32_NULL:
        return TYPE_INT_NULL;
        break;
    case DT_F64_NULL:
        return TYPE_FLOAT_NULL;
        break;
    case DT_U8_NULL:
        return TYPE_STRING_NULL;
        break;
    default:
        fprintf(stderr, "sym_get_type: something went wrong\n");
        return -1;
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
    case DT_I32: //i32
        param.type = TYPE_INT;
        break;
    case DT_F64: //f64
        param.type = TYPE_FLOAT;
        break;
    case DT_U8: //u8
        param.type = TYPE_STRING;
        break;
    case DT_VOID: //void
        param.type = TYPE_VOID;
        break;
    case DT_BOOL: //bool
        param.type = TYPE_BOOL;
        break;
    case DT_UNDEFINED: //odvozeny?
        param.type = TYPE_UNDEFINED;
        break;
    case DT_I32_NULL:
        param.type = TYPE_INT_NULL;
        break;
    case DT_F64_NULL:
        param.type = TYPE_FLOAT_NULL;
        break;
    case DT_U8_NULL:
        param.type = TYPE_STRING_NULL;
        break;
    default:
        fprintf(stderr, "sym_push_params something went wrong\n");
        break;
    }

    push_parameter(table, func_key, param);  

    sym_push_params(table, func_key, param_node->third); // rekurzivne volam

    return;
}


void consume_buffer(TokenBuffer* token, size_t n){
    for (size_t i = 0; i < n; i++)
    {
        // printf("token: %s\n",get_token_name(token->first->type));
        if (token->first->type == T_String) free(token->first->value.stringVal);
        if (token->first->type == T_ID) free(token->first->value.ID_name);
        free(token->first);
        token->first = token->second;
        token->second = token->third;
        token->third = token->fourth;
        token->fourth = scan();
        if (token->fourth->type == T_ERORR) {
            fprintf(stderr, "\nERROR TOKEN DETECTED!\n\n");
            parse_wrapper_ThrowError(99);
        }
    }
    
}

void buffer_check_first(TokenBuffer* token, token_type num){
    if (token->first->type != num)
    {
        fprintf(stderr, "\nExpected token type: %s got: %s\n", get_token_name(num), get_token_name(token->first->type));
        fprintf(stderr, "next tokens:\n");
        fprintf(stderr, "%s\n", get_token_name(token->second->type));
        fprintf(stderr, "%s\n", get_token_name(token->third->type));
        fprintf(stderr, "%s\n\n", get_token_name(token->fourth->type));
        parse_wrapper_ThrowError(2);
    }
    consume_buffer(token, 1);
    return;
}

/**
 * NODE ALOCATION
 */

Node * IdNode_new(char* id_string){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        parse_wrapper_ThrowError(99);
    }

    node_stack_push(PARSER_NODE_STACK, x);

    
    x->type = Id_N;
    x->data.id = create_string(id_string); 
    // free(id_string);
    return x;
} 

Node * StringNode_new(char *string){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        parse_wrapper_ThrowError(99);
    }

    node_stack_push(PARSER_NODE_STACK, x);


    x->type = Str_N;
    x->data.str = create_string(string);
    // free(string);
    return x;
}
Node * FloatNode_new(double num){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        parse_wrapper_ThrowError(99);
    }


    node_stack_push(PARSER_NODE_STACK, x);


    x->type = Float_N;
    x->data.flt = num;
    return x;
}
Node * IntNode_new(int num){
    Node* x = malloc(sizeof(Node));
    if (x == NULL)
    {
        parse_wrapper_ThrowError(99);
    }

    node_stack_push(PARSER_NODE_STACK, x);


    x->type = Int_N;
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
        parse_wrapper_ThrowError(99);
    }

    node_stack_push(PARSER_NODE_STACK, x);

    
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
        parse_wrapper_ThrowError(99);
    }

    node_stack_push(PARSER_NODE_STACK, x);


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
        parse_wrapper_ThrowError(99);
    }

    node_stack_push(PARSER_NODE_STACK, x);


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
        parse_wrapper_ThrowError(99);
    }

    node_stack_push(PARSER_NODE_STACK, x);


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
        parse_wrapper_ThrowError(99);
    }

    node_stack_push(PARSER_NODE_STACK, x);


    x->type = node_type;
    x->first = first;
    x->second = second;
    x->third = third;
    x->fourth = fourth;
    return x;
}

/**
 * RECURSIVE DESCEND
 */

Node * Parse_start(TokenBuffer* token){
    // cleanup if error
    PARSER_TOKEN_BUFFER = token;
    PARSER_NODE_STACK = node_stack_init();

    // prefill global variables (null as global variable)
    String* temp = create_string("null");
    insert_variable(token->sym_table, temp, TYPE_NULL, true);
    free_string(temp);
    
    Node * first = Parse_prolog(token);
    Node * second = Parse_program(token);

    Node * ret = TwoChildNode_new(Start_N,first, second);

    node_stack_free_keep_nodes(PARSER_NODE_STACK);

    return ret;
}

Node * Parse_id(TokenBuffer* token){
    if (token->first->type == T_ID)
    {
        // char* id_string = token->first->value.ID_name;
        Node * ret = IdNode_new(token->first->value.ID_name);
        consume_buffer(token, 1);
        return ret;
    }
    else
    {
        buffer_check_first(token, T_ID);
        return NULL;
    }
    
}
Node * Parse_string(TokenBuffer* token){
    if (token->first->type == T_String)
    {
        // char* string = token->first->value.stringVal;
        Node* ret = StringNode_new(token->first->value.stringVal);
        consume_buffer(token, 1);
        return ret;
    }
    else
    {
        buffer_check_first(token, T_String);
        return NULL;
    }
}

Node * Parse_prolog(TokenBuffer* token){
    buffer_check_first(token, T_const);
    if (token->first->type == T_ID){ //check if ifj
        if (strcmp(token->first->value.ID_name, "ifj")) parse_wrapper_ThrowError(2); // if id is not ifj
    }
    buffer_check_first(token, T_ID); 
    buffer_check_first(token, T_Assign);
    buffer_check_first(token, T_At);
    buffer_check_first(token, T_import);
    buffer_check_first(token, T_L_Round_B);
    if (token->first->type == T_String){ //check import file name
        if (strcmp(token->first->value.stringVal, "ifj24.zig")) parse_wrapper_ThrowError(2); // if name bad
    }
    buffer_check_first(token, T_String);
    buffer_check_first(token, T_R_Round_B);
    buffer_check_first(token, T_SemiC);

    /**
     * prefill sym table with ifj.functions
     */
    String* temp = create_string("ifj.readstr");
    insert_function(token->sym_table, temp, TYPE_STRING_NULL);
    free_string(temp);

    temp = create_string("ifj.readi32");
    insert_function(token->sym_table, temp, TYPE_INT_NULL);
    free_string(temp);

    temp = create_string("ifj.readf64");
    insert_function(token->sym_table, temp, TYPE_FLOAT_NULL);
    free_string(temp);

    temp = create_string("ifj.write");
    insert_function(token->sym_table, temp, TYPE_VOID);
    FunctionParam param;
    param.name = create_string("term");
    param.type = TYPE_UNDEFINED;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    free_string(temp);

    temp = create_string("ifj.i2f");
    insert_function(token->sym_table, temp, TYPE_FLOAT);
    param.name = create_string("term");
    param.type = TYPE_INT;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    free_string(temp);

    temp = create_string("ifj.f2i");
    insert_function(token->sym_table, temp, TYPE_INT);
    param.name = create_string("term");
    param.type = TYPE_FLOAT;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    free_string(temp);

    temp = create_string("ifj.string");
    insert_function(token->sym_table, temp, TYPE_STRING);
    param.name = create_string("term");
    param.type = TYPE_STRING;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    free_string(temp);

    temp = create_string("ifj.length");
    insert_function(token->sym_table, temp, TYPE_INT);
    param.name = create_string("s");
    param.type = TYPE_STRING;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    free_string(temp);

    temp = create_string("ifj.concat");
    insert_function(token->sym_table, temp, TYPE_STRING);
    param.name = create_string("s1");
    param.type = TYPE_STRING;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    param.name = create_string("s2");
    param.type = TYPE_STRING;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    free_string(temp);

    temp = create_string("ifj.substring");
    insert_function(token->sym_table, temp, TYPE_STRING_NULL);
    param.name = create_string("s");
    param.type = TYPE_STRING;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    param.name = create_string("i");
    param.type = TYPE_INT;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    param.name = create_string("j");
    param.type = TYPE_INT;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    free_string(temp);

    temp = create_string("ifj.strcmp");
    insert_function(token->sym_table, temp, TYPE_INT);
    param.name = create_string("s1");
    param.type = TYPE_STRING;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    param.name = create_string("s2");
    param.type = TYPE_STRING;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    free_string(temp);

    temp = create_string("ifj.ord");
    insert_function(token->sym_table, temp, TYPE_INT);
    param.name = create_string("s");
    param.type = TYPE_STRING;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    param.name = create_string("i");
    param.type = TYPE_INT;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    free_string(temp);

    temp = create_string("ifj.chr");
    insert_function(token->sym_table, temp, TYPE_STRING); 
    param.name = create_string("s1");
    param.type = TYPE_INT;
    param.next = NULL;
    push_parameter(token->sym_table, temp, param);
    free_string(param.name);
    free_string(temp);
    

    return NoChildNode_new(ProgramProlog_N);
}
Node * Parse_program(TokenBuffer* token){
    if (token->first->type == T_EOF)
    {
        return NULL;
    }

    Node* x = Parse_func_define(token);
    // x->data.sym_table = NULL;
    Node* y = Parse_program(token);

    return TwoChildNode_new(Program_N, x, y);
}

Node * Parse_datatype(TokenBuffer* token){
    int x = 0;
    if (token->first->type == T_Question) //null data types
    {
        consume_buffer(token, 1);
        x = x + 5;
    }
    
    switch (token->first->type)
    {
    case T_i32:
        consume_buffer(token,1);
        x = x + 1;
        break;
    
    case T_f64:
        consume_buffer(token, 1);
        x = x + 2;
        break;
        
    case T_L_Square_B:
        buffer_check_first(token, T_L_Square_B);
        if (token->first->type == T_Underscore) consume_buffer(token, 1); // allow [_]u8
        buffer_check_first(token,T_R_Square_B);
        buffer_check_first(token, T_u8);
        x = x + 3;
        break;
    
    case T_void:
        consume_buffer(token, 1);
        x = 4;
        break;

    default:
        fprintf(stderr, "Expected data_type_token, got: %s\n",get_token_name(token->first->type));
        parse_wrapper_ThrowError(2);
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

    insert_function(token->sym_table, a->data.id, sym_get_type(c->data.data_type));
    sym_push_params(token->sym_table, a->data.id, b);
    

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
    case T_Underscore: // allow assign to underscore
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
        fprintf(stderr, "expected statement_token got: %s \n",get_token_name(token->first->type));
        parse_wrapper_ThrowError(2);
        break;
    }

    return OneChildNode_new(Statement_N, a);
}

Node* Parse_rhs(TokenBuffer* token){

    if (token->first->type == T_String)
    {
        return Parse_string(token);
    }
    else return Parse_expression(token);

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
        parse_wrapper_ThrowError(2);
    }
    
    a = Parse_id(token);

    /** odvozeny typ z fce */
    if (token->first->type == T_Assign)
    {  
        b = NoChildNode_new(DataType_N);
        b->data.data_type = -1;
    }
    else {
        buffer_check_first(token, T_Colon);
        b = Parse_datatype(token);
    }

    buffer_check_first(token, T_Assign);

    c = Parse_rhs(token);

    buffer_check_first(token, T_SemiC);

    Node * ret = ThreeChildNode_new(VariableDefine_N, a, b, c);
    ret->data.var_or_const = var_const;

    return ret;
}


Node * Parse_variable_assign(TokenBuffer* token){

    // allow assingment to _ (undersore)
    Node * a;
    if (token->first->type == T_Underscore){
        a = NoChildNode_new(Underscore_N);
        consume_buffer(token, 1);
    }
    else a = Parse_id(token);

    buffer_check_first(token, T_Assign);
    Node * b = Parse_rhs(token);

    buffer_check_first(token, T_SemiC);
    return TwoChildNode_new(VariableAssign_N, a, b);
}

Node * Parse_func_call(TokenBuffer* token){
    bool is_ifj = false;
    String* ifj;
    if (token->second->type == T_Dot)
    {
        ifj = create_string(token->first->value.ID_name);
        buffer_check_first(token, T_ID); 
        buffer_check_first(token, T_Dot);
        is_ifj = true;
    }
    
    Node * a = Parse_id(token); 

    if (!strcmp(a->data.id->data, "null")) parse_wrapper_ThrowError(2);

    buffer_check_first(token, T_L_Round_B);
    Node * b = Parse_params(token);
    buffer_check_first(token, T_R_Round_B);

    if (is_ifj)
    {
        String* dot = create_string(".");
        String* ifj_dot = concat_strings(ifj, dot);
        String* tmp = a->data.id;
        a->data.id = concat_strings(ifj_dot, tmp);
        free_string(dot);
        free_string(ifj_dot);
        free_string(tmp);
        free_string(ifj);
    }
    

    return TwoChildNode_new(FuncCall_N, a, b);
}

Node * Parse_rhs_param(TokenBuffer * token){
    if (token->first->type == T_String)
    {
        return Parse_string(token);
    }
    else 
    {
        return Parse_expression(token);
    }
}

Node * Parse_params(TokenBuffer* token){
    if (token->first->type == T_R_Round_B)
    {
        return NULL;
    }

    Node * a = Parse_rhs_param(token);
    
    Node * b = Parse_params_next(token);

    return TwoChildNode_new(Params_N, a, b);
}

Node * Parse_params_next(TokenBuffer* token){
    if (token->first->type == T_R_Round_B)
    {
        return NULL;
    }

    buffer_check_first(token, T_Comma);
    
    Node * a = Parse_rhs_param(token);
    
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
    String* ifj;
    if (token->second->type == T_Dot)
    {   
        ifj = create_string(token->first->value.ID_name);
        buffer_check_first(token, T_ID);
        buffer_check_first(token, T_Dot);
        is_ifj = true;
    }
    
    Node * a = Parse_id(token);

    if (!strcmp(a->data.id->data, "null")) parse_wrapper_ThrowError(2);

    buffer_check_first(token, T_L_Round_B);
    Node * b = Parse_params(token);
    buffer_check_first(token, T_R_Round_B);
    buffer_check_first(token, T_SemiC);

    if (is_ifj)
    {
        String* dot = create_string(".");
        String* ifj_dot = concat_strings(ifj, dot);
        String* tmp = a->data.id;
        a->data.id = concat_strings(ifj_dot, tmp);
        free_string(dot);
        free_string(ifj_dot);
        free_string(tmp);
        free_string(ifj);
    }
    
    return TwoChildNode_new(VoidCall_N, a, b);
}

Node * Parse_return_statement(TokenBuffer* token){
    buffer_check_first(token, T_return);
    Node * a;

    if (token->first->type == T_SemiC)
    {
        a = NULL;
    }
    else
    {
        a = Parse_rhs(token);
    } 
    
    buffer_check_first(token, T_SemiC);

    return OneChildNode_new(ReturnStatement_N, a);
}

