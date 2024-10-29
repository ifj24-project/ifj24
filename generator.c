/** @file generator.c
* @author Daniel Geier (xgeierd00)
*
* IFJ24
*
* @brief Implementace generatoru
*/

#include "generator.h"
#include "error/error.h"
#include "symtable.h"
#include "parser/parser.h"

void generate(Node* node) {
    switch (node->type) {

      case Start_N:
        // Kod v jazyce IFJ24 zacina radkem
        printf(".IFJcode24\n");

        generate(node->first);
        generate(node->second);
        generate(node->third);
        generate(node->fourth);
        break;

      case ProgramProlog_N:
        break;

      case Program_N:



      case DataType_N:
      case FuncDefine_N:
      case ParamsDefine_N:
      case ParamsDefineNext_N:
      case FuncBody_N:
      case Statement_N:
      case VariableDefine_N:
      case VariableAssign_N:
      case FuncCall_N:
      case Params_N:
      case ParamsNext_N:
      case If_N:
      case While_N:
      case VoidCall_N:
      case ReturnStatement_N:
      case Id_N:
      case Str_N:
      case Float_N:
      case If_not_null:
      case while_not_null:
    // EXPRESSION
      case Expression_N:
      case Compared_N:
      case Term_N:
      case Factor_N:
      case Lesser_N:
      case LesserEq_N:
      case Greater_N:
      case GreaterEq_N:
      case Eq_N:
      case NotEq_N:
      case Plus_N:
      case Minus_N:
      case Times_N:
      case Divide_N:

      default:
        break;
    }
}