/** @file generator.c
* @author Daniel Geier (xgeierd00)
* @autor Patrik Mokrusa (xmokrup00)
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


/**
 * @brief Funkce pro generovani kodu
 *
 * @param node ukazatel na uzel stromu
 */
void generate(Node* node)
{
    if (node == NULL) return;
    //static int label_counter = 0;

    switch (node->type)
    {
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
        if (node->third->data.data_type == DT_VOID)
        {
            // void function
            // if main
            // asi nemusi byt muzu proste nazacatku udelat main je kontrolovano jestli existuje
            if (strcmp(node->first->data.id->data, "main") == 0)
            {
                printf("LABEL $$main\n");
                printf("CREATEFRAME\n");

                // globalni promena pro vraceni hodnot z funkci
                printf("DEFVAR GF@rvalue_return\n");
                // globalni promenna pro vraceni true/false
                printf("DEFVAR GF@bool_return\n");
                // globalni promenne pro expresions lhs a rhs
                printf("DEFVAR GF@lhs\n");
                printf("DEFVAR GF@rhs\n");

                generate(node->second);
                generate(node->third);
                generate(node->fourth);
            }
            else
            {
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
        }
        else
        {
            // non-void function
            printf("JUMP skip$%s\n", node->first->data.id->data);
            printf("LABEL $%s\n", node->first->data.id->data);
            printf("CREATEFRAME\n");
            printf("PUSHFRAME\n");


            printf("JUMP vardef$%s\n", node->first->data.id->data);
            printf("LABEL vardef$%s$back\n", node->first->data.id->data);

            // postupne projit nody s parametry a pricitat pocet parametru
            int pocet_param = 0;
            Node* node_params = node;
            while (node_params->second != NULL)
            {
                pocet_param++;
                node_params = node_params->second;
            }

            // Popne vsechny parametry od konce
            for (int i = pocet_param; i > 0; i--)
            {
                printf("POPS LF@%s\n", node_params->first->data.id->data);
            }


            // generate vsechny params a pak do nich popnout hodnoty zasobniku pozpatku

            // zjistit pocet params
            // pop all params
            //generate(node->second);

            generate(node->third);
            generate(node->fourth);

            // sem skoci vsechny returny funkce
            printf("# Return funkce\n");
            printf("LABEL return$%s\n", node->first->data.id->data);

            // return value ulozit na zasobnik
            printf("POPS GF@return_value\n");


            printf("POPFRAME\n");
            printf("RETURN\n\n");

            printf("# Definice promennych\n");
            printf("LABEL vardef$%s\n", node->first->data.id->data);
            // define all params
            generate(node->second);
            printf("JUMP vardef$%s$back\n", node->first->data.id->data);

            printf("LABEL skip$%s", node->first->data.id->data);
        }
        break;

    case ParamsDefine_N:
        printf("DEFVAR LF@%s\n", node->first->data.id->data);
        generate(node->second);
        generate(node->third);
        break;

    case ParamsDefineNext_N:
        printf("DEFVAR LF@%s\n", node->first->data.id->data);
        generate(node->second);
        generate(node->third);
        break;

    case FuncBody_N:
        generate(node->first);
        generate(node->second);
        break;

    case Statement_N:
        generate(node->first);
        break;

    case VariableDefine_N:
        // if node->third == EXPRESSION -> generate_expr
        if (node->third->type == Expression_N)
        {
            printf("DEFVAR LF@%s ", node->first->data.id->data);
            generate_expr(node->third, node->third->data.data_type);
        }
        else
        {
            printf("DEFVAR LF@%s\n", node->first->data.id->data);
            generate(node->third);
        }
        printf("DEFVAR LF@%s\n", node->first->data.id->data);
    //printf("MOVE LF@%s %s@%s", node->first->data.id->data, data_type(node->second->data.data_type), node->third->data.id->data);
        printf("MOVE LF@%s ", node->first->data.id->data);
        generate(node->second);
        break;

    case VariableAssign_N:
        printf("MOVE LF@%s ", node->first->data.id->data);
        generate(node->second);
        break;

    case FuncCall_N:
        printf("JUMP $%s\n", node->first->data.id->data);
        generate(node->second);
        break;

    case Params_N:
        // pushs parametr
        printf("PUSHS %s@%s\n", data_type(node->first->data.id->data), node->first->data.id->data);
        generate(node->second);
        break;

    case ParamsNext_N:
        printf("PUSHS %s@%s\n", data_type(node->first->data.id->data), node->first->data.id->data);
        generate(node->second);
        break;

    case If_N:
        printf("PUSHS LF@%s\n", node->first->data.id->data);

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
void generate_expr(Node* node, VarType expr_type)
{
    if (node == NULL) return; // break

    // prefix notation
    switch (node->type)
    {
    case Id_N:
        // TODO: nejak tam tu promennou dat misto "symbol"
        printf("PUSHS   LF@%s\n", node->data.id->data);
        break;

    case Float_N:
        printf("PUSHS float@%f\n", node->data.flt);
        break;

    case Int_N:
        printf("PUSHS int@%d\n", node->data.integer);
        break;

    case Lesser_N:
        generate_expr(node->first, expr_type);
        generate_expr(node->second, expr_type);
        printf("LTS\n");
        break;

    case LesserEq_N:
        generate_expr(node->first, expr_type);
        generate_expr(node->second, expr_type);
        printf("GTS NOTS\n");
        break;

    case Greater_N:
        generate_expr(node->first, expr_type);
        generate_expr(node->second, expr_type);
		printf("GTS\n");
        break;

    case GreaterEq_N:
        generate_expr(node->first, expr_type);
        generate_expr(node->second, expr_type);
    // do something
        printf("LTS NOTS");
        break;

    case Eq_N:
        generate_expr(node->first, expr_type);
        generate_expr(node->second, expr_type);
    // do something
        printf("EQS");
        break;

    case NotEq_N:
        generate_expr(node->first, expr_type);
        generate_expr(node->second, expr_type);
    // do something
        printf("EQS NOTS");
        break;

    case Plus_N:
        generate_expr(node->first, expr_type);
        generate_expr(node->second, expr_type);
        printf("ADDS");
        break;

    case Minus_N:
        generate_expr(node->first, expr_type);
        generate_expr(node->second, expr_type);
        printf("SUBS");
        break;

    case Times_N:
        generate_expr(node->first, expr_type);
        generate_expr(node->second, expr_type);
        printf("MULS");
        break;

    case Divide_N:
        generate_expr(node->first, expr_type);
        generate_expr(node->second, expr_type);

    // TODO: nejak zkontrolovat jestli pouzit DIV nebo IDIV
        // if % then IDIVS else DIVS

        printf("DIVS");

        break;

    default:
        ThrowError(99);
        break;
    }

    return;
}

char* data_type(char* type)
{
    /*
    // if i32 or ?i32
    if (strcmp(type, "i32") == 0 || strcmp(type, "?i32") == 0) {
      return "int";
    }
    // if f64 or ?f64
    else if (strcmp(type, "f64") == 0 || strcmp(type, "?f64") == 0) {
      return "float";
    }
    // if []u8 or ?[]u8
    else if (strcmp(type, "[]u8") == 0 || strcmp(type, "?[]u8") == 0) {
      return "string";
    }
    else {
      return "nil";
    }
    */

    switch (type)
    {
    case DT_I32: //i32
        return "int";
        break;
    case DT_F64: //f64
        return "float";
        break;
    case DT_U8: //u8
        return "string";
        break;
    case DT_VOID: //void
        return "void";
        break;
    case DT_BOOL: //bool
        return "bool";
        break;
    case DT_UNDEFINED: //odvozeny?
        return "";
        break;
    // jak to je s tim nil?
    case DT_I32_NULL:
        return "nil";;
        break;
    case DT_F64_NULL:
        return "nil";
        break;
    case DT_U8_NULL:
        return "nil";
        break;
    default:
        return "";
        break;
    }

    return;
}
