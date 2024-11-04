/** @file generator.c
* @author Daniel Geier (xgeierd00)
* @autor Patrik Mokrusa (xmokru00)
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

// TODO: zabalit fnc generate do dalsi, ktera bude kontrolovat a vyvolavat errory
// pripadne pak upravit header soubor

void generate(Node *node) {
  if (node == NULL) return;
  //static int label_counter = 0;

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
        // asi nemusi byt muzu proste nazacatku udelat main je kontrolovano jestli existuje
        if (strcmp(node->first->data.id->data, "main") == 0) {
          printf("LABEL $$main\n");
          printf("CREATEFRAME\n");
        } else {
          printf("JUMP skip$%s\n", node->first->data.id->data);
          printf("LABEL $%s\n", node->first->data.id->data);
          printf("CREATEFRAME\n");
          printf("PUSHFRAME\n");

          generate(node->second);
          generate(node->third);
          generate(node->fourth);

          printf("POPFRAME\n");
          printf("RETURN\n\n");
          printf("LABEL skip$%s", node->first->data.id->data);
        }
      } else {
        // non-void function
          printf("JUMP skip$%s\n", node->first->data.id->data);
          printf("LABEL $%s\n", node->first->data.id->data);
          printf("CREATEFRAME\n");
          printf("PUSHFRAME\n");
          printf("JUMP vardef$%s\n", node->first->data.id->data);

          // zjistit pocet params
          // pop all params
          //generate(node->second);

          generate(node->third);
          generate(node->fourth);

          // sem skoci vsechny returny funkce
          printf("LABEL return$%s\n", node->first->data.id->data);

          // return value

          printf("POPFRAME\n");
          printf("RETURN\n\n");

          printf("LABEL vardef$%s\n", node->first->data.id->data);
          // define all params
          //generate(node->second);

          printf("LABEL skip$%s", node->first->data.id->data);
      }
      break;

    case ParamsDefine_N:
      //printf("DEFVAR LF@%s\n", node->first->data.id->data);
      generate(node->first);
      generate(node->second);
      generate(node->third);
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
      // pushnout pocet parametru
      generate(node->second);
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

    // case Lesser_N:
    //   generate(node->first);
    //   generate(node->second);
    //   generate(node->third);
    //   generate(node->fourth);
    //   break;

    // case LesserEq_N:
    //   generate(node->first);
    //   generate(node->second);
    //   generate(node->third);
    //   generate(node->fourth);
    //   break;

    // case Greater_N:
    //   generate(node->first);
    //   generate(node->second);
    //   generate(node->third);
    //   generate(node->fourth);
    //   break;

    // case GreaterEq_N:
    //   generate(node->first);
    //   generate(node->second);
    //   generate(node->third);
    //   generate(node->fourth);
    //   break;

    // case Eq_N:
    //   generate(node->first);
    //   generate(node->second);
    //   generate(node->third);
    //   generate(node->fourth);
    //   break;

    // case NotEq_N:
    //   generate(node->first);
    //   generate(node->second);
    //   generate(node->third);
    //   generate(node->fourth);
    //   break;

    // case Plus_N:
    //   generate(node->first);
    //   generate(node->second);
    //   generate(node->third);
    //   generate(node->fourth);
    //   break;

    // case Minus_N:
    //   generate(node->first);
    //   generate(node->second);
    //   generate(node->third);
    //   generate(node->fourth);
    //   break;

    // case Times_N:
    //   generate(node->first);
    //   generate(node->second);
    //   generate(node->third);
    //   generate(node->fourth);
    //   break;

    // case Divide_N:
    //   generate(node->first);
    //   generate(node->second);
    //   generate(node->third);
    //   generate(node->fourth);
    //   break;

    default:
      generate(node->first);
      generate(node->second);
      generate(node->third);
      generate(node->fourth);
      break;
  }
  // Exit succes
  //printf("EXIt in@0\n");
}



/**
 * doporucuju udelat ty expressions zvlast
 * pak byt postorderem prochazel
 * 
 * TODO: vymenit "var" v printech za nejaky temp register
 */
void generate_expr(Node* node, VarType expr_type){
  if (node == NULL) return; // break

  switch (node->type)
  {
   case Id_N:
      // TODO: nejak tam tu promennou dat misto "symbol"
      printf("PUSHS   symbol\n");
      break;

    case Float_N:
      printf("PUSHS %f\n", node->data.flt);
      break;

    case Int_N:
      printf("PUSHS %d\n", node->data.integer);
      break;

    case Lesser_N:
      generate_expr(node->first, expr_type);
      generate_expr(node->second, expr_type);
      // do something
      break;

    case LesserEq_N:
      generate_expr(node->first, expr_type);
      generate_expr(node->second, expr_type);
      // do something
      break;

    case Greater_N:
      generate_expr(node->first, expr_type);
      generate_expr(node->second, expr_type);
      // do something
      break;

    case GreaterEq_N:
      generate_expr(node->first, expr_type);
      generate_expr(node->second, expr_type);
      // do something
      break;

    case Eq_N:
      generate_expr(node->first, expr_type);
      generate_expr(node->second, expr_type);
      // do something
      break;

    case NotEq_N:
      generate_expr(node->first, expr_type);
      generate_expr(node->second, expr_type);
      // do something
      break;

    case Plus_N:
      generate_expr(node->first, expr_type);
      generate_expr(node->second, expr_type);
      printf("ADD var \n");
      break;

    case Minus_N:
      generate_expr(node->first, expr_type);
      generate_expr(node->second, expr_type);
      printf("SUB var \n");
      break;

    case Times_N:
      generate_expr(node->first, expr_type);
      generate_expr(node->second, expr_type);
      printf("MUL var \n");
      break;

    case Divide_N:
      generate_expr(node->first, expr_type);
      generate_expr(node->second, expr_type);

      // TODO: nejak zkontrolovat jestli pouzit DIV nebo IDIV
      printf("DIV var \n");
      break;

    default:
      ThrowError(99);
      break;
  }

return;
}