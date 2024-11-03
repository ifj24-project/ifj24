/** @file generator.c
* @author Daniel Geier (xgeierd00)
*
* IFJ24
*
* @brief Implementace generatoru
*/

#include "generator.h"

#include <string.h>

#include "error/error.h"
#include "symtable.h"
#include "parser/parser.h"

void generate(Node *node) {
  if (node == NULL) return;
  static int label_counter = 0;

  switch (node->type) {
    case Start_N:
      generate(node->first);
      generate(node->second);
      break;

    case ProgramProlog_N:
      // Kod v jazyce IFJ24 zacina radkem
      printf(".IFJcode24\n\nJUMP $$main\n");
      break;

    case Program_N:
      generate(node->first);
      generate(node->second);

      break;


    case DataType_N:
      break;

    case FuncDefine_N:
      if (node->third->data.data_type == DT_VOID) {
        // void function
        // if main
        if (strcmp(node->first->data.id->data, "main") == 0) {
          printf("LABEL $$main\n");
          printf("CREATEFRAME\n");
        } else {
          printf("JUMP skip$%d\n", label_counter++);
          printf("LABEL $%s\n", node->first->data.id->data);
          printf("CREATEFRAME\n");

          generate(node->second);
          generate(node->third);
          generate(node->fourth);

          printf("POPFRAME\n");
          printf("RETURN\n\n");
        }
      } else {
        // non-void function
          printf("JUMP skip$%d\n", label_counter++);
          printf("LABEL $%s\n", node->first->data.id->data);
          printf("PUSHFRAME\n");
          printf("JUMP vardef$%s%d\n", node->first->data.id->data, label_counter);
          // pop all params

          generate(node->second);
          generate(node->third);
          generate(node->fourth);

          printf("LABEL return$%s%d\n", node->first->data.id->data, label_counter);

          // return value

          printf("POPFRAME\n");
          printf("RETURN\n\n");

          printf("LABEL vardef$%s%d\n", node->first->data.id->data, label_counter);
          // define all params
      }
      break;

    case ParamsDefine_N:
      generate(node->first);
      generate(node->second);
      break;

    case ParamsDefineNext_N:
      generate(node->first);
      generate(node->second);
      break;

    case FuncBody_N:
      generate(node->first);
      generate(node->second);
      break;

    case Statement_N:
      generate(node->first);
      break;

    case VariableDefine_N:
      generate(node->first);
      generate(node->second);
      break;

    case VariableAssign_N:
      generate(node->first);
      generate(node->second);
      break;

    case FuncCall_N:
      printf("JUMP $%s\n", node->first->data.id->data);
      generate(node->first);
      break;

    case Params_N:
      generate(node->first);
      generate(node->second);
      break;

    case ParamsNext_N:
      generate(node->first);
      generate(node->second);
      break;

    case If_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      break;

    case While_N:
      generate(node->first);
      generate(node->second);
      break;

    case VoidCall_N:
      generate(node->first);
      break;

    case ReturnStatement_N:
      generate(node->first);
      break;

    case Id_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case Str_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case Float_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case If_not_null:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case while_not_null:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    // EXPRESSION
    case Expression_N:
      generate(node->first);
      generate(node->second);
      break;

    case Compared_N:
      generate(node->first);
      generate(node->second);
      break;

    case Term_N:
      generate(node->first);
      generate(node->second);
      break;

    case Factor_N:
      generate(node->first);
      break;

    case Lesser_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case LesserEq_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case Greater_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case GreaterEq_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case Eq_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case NotEq_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case Plus_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case Minus_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case Times_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    case Divide_N:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;

    default:
      break;
  }
  // Exit succes
  //printf("EXIt in@0\n");
}
