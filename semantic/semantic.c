 
#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../error/error.h"

void semantic_scan(Node* node){
    switch (node->type)
    {
    case Start_N:
        
        break;
    case ProgramProlog_N:
        
        break;
    case Program_N:
        
        break;
    case DataType_N:
        
        break;
    case FuncDefine_N:
        
        break;
    case ParamsDefine_N:
        
        break;
    case ParamsDefineNext_N:
        
        break;
    case FuncBody_N:
        
        break;
    case Statement_N:
        
        break;
    case VariableDefine_N:
        
        break;
    case VariableAssign_N:
        
        break;
    case FuncCall_N:
        
        break;
    case Params_N:
        
        break;
    case ParamsNext_N:
        
        break;
    case If_N:
        
        break;
    case While_N:
        
        break;
    case VoidCall_N:
        
        break;
    case ReturnStatement_N:
        
        break;
    case Id_N:
        
        break;
    case Str_N:
        
        break;
    case Float_N:
        
        break;
    case If_not_null:
        
        break;
        
    case while_not_null:
        
        break;
    
    default:
        break;
    }
}