 
#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../error/error.h"

/**
 * TODO:
 * return
 * include expr assignment
 * include getting type from expr
 * 
 * mark vars as used
 * include unused var check (symtable function)
 * 
 * if only true false expr
 * while check contition (viz. zadani)
 * 
 * expr:
 * give expr type
 * convert to compatible types
 * null only compatible in == and !=
 * 
 */


void semantic_scan(Node* node, SymbolTable* global_table, String* global_func_key, SymbolTable* local_table){
    if (node == NULL)
    {
        return;
    }
    FunctionInfo* func_info;
    VariableInfo* var_info;
    FunctionParam* def_param;
    Node* called_param;
    
    switch (node->type)
    {
    case Start_N:
        String* temp = create_string("main");
        FunctionInfo* info = find_symbol(global_table, temp);
        if (info == NULL) ThrowError(3); // main doesnt exits
        if (info->param_count != 0) ThrowError(4); // main has parametrs
        free_string(temp);

        semantic_scan(node->second, global_table, global_func_key,local_table);
        
        break;

    case FuncDefine_N:
        
        func_info = find_symbol(global_table, node->first->data.id);
        def_param = func_info->params;
        local_table = create_symbol_table(100);
        while (def_param != NULL)
        {
            insert_variable(local_table, def_param->name, def_param->type, false);

            def_param = def_param->next;
        }
        
        semantic_scan(node->fourth, global_table, node->first->data.id, local_table);
        free_symbol_table(local_table);
        local_table = NULL;
        break;

    case VariableDefine_N:
        // redefinice
        if (find_symbol(local_table, node->first->data.id) != NULL || find_symbol(global_table, node->first->data.id) != NULL) 
        {
            ThrowError(5);
        }

        //odvodit typ z funkce
        if (node->third->type == FuncCall_N)
        {
            func_info = find_symbol(global_table, node->third->first->data.id);

            switch (func_info->return_type)
            {
            case TYPE_INT:
                node->second->data.data_type = DT_I32;
                break;
            case TYPE_INT_NULL:
                node->second->data.data_type = DT_I32_NULL;
                break;
            case TYPE_FLOAT:
                node->second->data.data_type = DT_F64;
                break;
            case TYPE_FLOAT_NULL:
                node->second->data.data_type = DT_F64_NULL;
                break;
            case TYPE_STRING:
                node->second->data.data_type = DT_U8;
                break;
            case TYPE_STRING_NULL:
                node->second->data.data_type = DT_U8_NULL;
                break;
            case TYPE_UNDEFINED:
                node->second->data.data_type = DT_UNDEFINED;
                break;
            default:
                ThrowError(8);
                break;
            }
        }

        // TODO odvodit typ z vyrazu
        
        insert_variable(local_table, node->first->data.id, sym_get_type(node->second->data.data_type), node->data.var_or_const);
        
        semantic_scan(node->third, global_table, global_func_key, local_table); // co se tam assignuje
        break;
    case VariableAssign_N:
        // function not defined
        var_info = find_symbol(local_table, node->first->data.id);
        if (var_info == NULL)
        {
            ThrowError(3);
        }
        else if (var_info->is_const == true)
        {
            ThrowError(5);
        }

        if (node->second->type == Expression_N)
        {
            // TODO: check expr type
        }
        else if (node->second->type == Str_N)
        {
            if (var_info->data_type != TYPE_STRING_NULL && var_info->data_type != TYPE_STRING) ThrowError(7);
        }
        
        else if (node->second->type == FuncCall_N)
        {
            func_info = find_symbol(global_table, node->second->first->data.id);
            if (func_info != NULL) // if func is defined
            {
                // assign types not compatible
                switch (var_info->data_type)
                {
                case TYPE_INT:
                    if (func_info->return_type != TYPE_INT) ThrowError(7);
                    break;
                case TYPE_INT_NULL:
                    if (func_info->return_type != TYPE_INT_NULL && func_info->return_type != TYPE_INT) ThrowError(7);
                    break;
                case TYPE_FLOAT:
                    if (func_info->return_type != TYPE_FLOAT) ThrowError(7);
                    break;
                case TYPE_FLOAT_NULL:
                    if (func_info->return_type != TYPE_FLOAT_NULL && func_info->return_type != TYPE_FLOAT) ThrowError(7);
                    break;
                case TYPE_STRING:
                    if (func_info->return_type != TYPE_STRING) ThrowError(7);
                    break;
                case TYPE_STRING_NULL:
                    if (func_info->return_type != TYPE_STRING_NULL && func_info->return_type != TYPE_STRING) ThrowError(7);
                    break;
                case TYPE_UNDEFINED:
                    // undefined loophole
                    break;
                default:
                    ThrowError(7);
                    break;
                }
            }
        }
        
        semantic_scan(node->third, global_table, global_func_key, local_table); // co se tam assignuje
        break;
    case FuncCall_N:
        // function not defined
        if (find_symbol(global_table, node->first->data.id) == NULL)
        {
            ThrowError(3);
        }

        // check for error 4 (check params)
        func_info = find_symbol(global_table, node->first->data.id);
        def_param = func_info->params;
        called_param = node->second;
        while (def_param != NULL && called_param != NULL)
        {
            if (def_param->type != TYPE_UNDEFINED) // takes any type
            {
                if (called_param->first->type == Str_N)
                {
                    if (def_param->type != TYPE_STRING || def_param->type != TYPE_STRING_NULL) ThrowError(4);
                }
                else if (called_param->first->type == Id_N)
                {
                    if (def_param->type)
                    {
                        var_info = find_symbol(local_table, called_param->first->data.id);
                        if (var_info == NULL) ThrowError(3);
                        // if (var_info->data_type != def_param->type) ThrowError(4);
                        if (var_info->data_type == def_param->type || var_info->data_type == TYPE_UNDEFINED)
                        {
                            // undefined loophole
                        }
                        else { ThrowError(4); }
                    }
                    
                }
                else if (called_param->first->type == Int_N)
                {
                    if (def_param->type != TYPE_INT || def_param->type != TYPE_INT_NULL) ThrowError(4);
                }
                else if (called_param->first->type == Float_N)
                {
                    if (def_param->type != TYPE_FLOAT || def_param->type != TYPE_FLOAT_NULL) ThrowError(4);
                }
                else if (called_param->first->type == Expression_N)
                {
                    //TODO: check expression type
                }
                else if (called_param->first->type == FuncCall_N)
                {
                    semantic_scan(called_param->first, global_table, global_func_key, local_table);
                }
            }
            else
            {
                if (called_param->first->type == Id_N && find_symbol(local_table, called_param->first->data.id) == NULL) ThrowError(3); // check if called param is defined
            }
            
            called_param = called_param->second;
            def_param = def_param->next;
        }

        if (def_param != NULL || called_param != NULL) // different num of params
        {
            ThrowError(4);
        }
        break;
    case If_N:
        if (node->data.has_not_null_id == true)
        {
            //TODO: get type from expr
            insert_variable(local_table, node->second->data.id, TYPE_UNDEFINED, node->data.var_or_const);
            semantic_scan(node->third, global_table, global_func_key, local_table);
            delete_symbol(local_table, node->second->data.id);
            semantic_scan(node->fourth, global_table, global_func_key, local_table);
        }
        else
        {
            semantic_scan(node->second, global_table, global_func_key, local_table);
            semantic_scan(node->third, global_table, global_func_key, local_table);
        }
        break;
    case While_N:
        if (node->data.has_not_null_id == true)
        {
            //TODO: get type from expr for pipe var
            insert_variable(local_table, node->second->data.id, TYPE_UNDEFINED, node->data.var_or_const); // TODO: |is var or const?| + remove loophole
            semantic_scan(node->third, global_table, global_func_key, local_table);
            delete_symbol(local_table, node->second->data.id);
        }
        else
        {
            semantic_scan(node->second, global_table, global_func_key, local_table);
        }
        break;
    case VoidCall_N:
        // function not defined
        if (find_symbol(global_table, node->first->data.id) == NULL)
        {
            ThrowError(3);
        }

        // check for error 4 (check params)
        func_info = find_symbol(global_table, node->first->data.id);
        def_param = func_info->params;
        called_param = node->second;
        while (def_param != NULL && called_param != NULL)
        {
            if (def_param->type != TYPE_UNDEFINED) // takes any type
            {
                if (called_param->first->type == Str_N)
                {
                    if (def_param->type != TYPE_STRING || def_param->type != TYPE_STRING_NULL) ThrowError(4);
                }
                else if (called_param->first->type == Id_N)
                {
                        var_info = find_symbol(local_table, called_param->first->data.id);
                        if (var_info == NULL) ThrowError(3);
                        // if (var_info->data_type != def_param->type) ThrowError(4);
                        if (var_info->data_type == def_param->type || var_info->data_type == TYPE_UNDEFINED)
                        {
                            // undefined loophole
                            // TODO: remove when if |pipes| is done
                        }
                        else { ThrowError(4); }
                }
                else if (called_param->first->type == Int_N)
                {
                    if (def_param->type != TYPE_INT || def_param->type != TYPE_INT_NULL) ThrowError(4);
                }
                else if (called_param->first->type == Float_N)
                {
                    if (def_param->type != TYPE_FLOAT || def_param->type != TYPE_FLOAT_NULL) ThrowError(4);
                }
                else if (called_param->first->type == Expression_N)
                {
                    //TODO: check expression type
                }
                else if (called_param->first->type == FuncCall_N)
                {
                    semantic_scan(called_param->first, global_table, global_func_key, local_table);
                }
            }
            else { // takes any type, check if defined
                if (called_param->first->type == Id_N && find_symbol(local_table, called_param->first->data.id) == NULL) ThrowError(3); // check if called param is defined
            }

            called_param = called_param->second;
            def_param = def_param->next;
        }

        if (def_param != NULL || called_param != NULL) // different num of params
        {
            ThrowError(4);
        }

        break;
    // case ReturnStatement_N:
    //     break;
    default:
        semantic_scan(node->first, global_table, global_func_key, local_table);
        semantic_scan(node->second, global_table, global_func_key, local_table);
        semantic_scan(node->third, global_table, global_func_key, local_table);
        semantic_scan(node->fourth, global_table, global_func_key, local_table);
        break;
    }

    return;
}




