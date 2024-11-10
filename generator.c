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
#include <stdbool.h>

#include "error/error.h"
#include "symtable.h"
#include "parser/parser.h"

// TODO: zabalit fnc generate do dalsi, ktera bude kontrolovat a vyvolavat errory
// pripadne pak upravit header soubor

// pomocna promenna pro definici promennych ve funkci a jejich nasledne popovani ze zasobniku
bool var_def = false;
// pomocna promenna jestli se nachazime uvnitr funkce
bool in_func = false;

void generate(Node *node) {
    if (node == NULL) return;
    //static int label_counter = 0;
    // pocet if statements
    static int if_counter = 0;
    // pocet while statements
    static int while_counter = 0;
    // pocet void calls
    static int void_call_counter = 0;

    switch (node->type) {
        case Start_N:
            generate(node->first);
            generate(node->second);
            break;

        case ProgramProlog_N:
            // Kod v jazyce mezikodu IFJ24 zacina radkem
            printf(".IFJcode24\n\n");
            break;

        case Program_N:
            generate(node->first);
            generate(node->second);

            break;


        case DataType_N:
            break;

        case FuncDefine_N:
            in_func = true;
            if (node->third->data.data_type == DT_VOID) {
                // void function
                // if main
                // asi nemusi byt muzu proste nazacatku udelat main je kontrolovano jestli existuje
                if (strcmp(node->first->data.id->data, "main") == 0) {
                    printf("JUMP $$main\n");
                    printf("LABEL $$main\n");
                    printf("CREATEFRAME\n");

                    // globalni promena pro vraceni hodnot z funkci
                    printf("DEFVAR GF@rvalue_return\n");
                    // globalni promenna pro vraceni true/false z expresion
                    printf("DEFVAR GF@exp_return\n");
                    // globalni promenne pro expresions lhs a rhs
                    printf("DEFVAR GF@lhs\n");
                    printf("DEFVAR GF@rhs\n");

                    generate(node->second);
                    generate(node->third);
                    generate(node->fourth);
                } else {
                    printf("JUMP skip$%s\n", node->first->data.id->data);
                    printf("LABEL $%s\n", node->first->data.id->data);
                    printf("CREATEFRAME\n");
                    printf("PUSHFRAME\n");

                    printf("JUMP vardef$%s\n", node->first->data.id->data);
                    printf("LABEL vardef$%s$back\n", node->first->data.id->data);

                    var_def = false;
                    generate(node->second);

                    in_func = true;
                    printf("# Telo funkce\n");
                    generate(node->third);
                    generate(node->fourth);

                    // sem skoci vsechny returny funkce
                    printf("# Return funkce\n");
                    printf("LABEL return$%s\n", node->first->data.id->data);

                    // ulozi return hodnotu do globalni promenne
                    printf("POPS GF@return_value\n");
                    // nebo necham tu hodnotu na zasobniku? -> tj nebudu ji popovat

                    printf("POPFRAME\n");
                    printf("RETURN\n\n");

                    printf("# Definice promennych\n");
                    printf("LABEL vardef$%s\n", node->first->data.id->data);
                    // define all params
                    var_def = true;
                    generate(node->second);

                    printf("JUMP vardef$%s$back\n", node->first->data.id->data);

                    printf("LABEL skip$%s", node->first->data.id->data);
                    in_func = false;
                }
            } else {
                // non-void function
                printf("JUMP skip$%s\n", node->first->data.id->data);
                printf("LABEL $%s\n", node->first->data.id->data);
                printf("CREATEFRAME\n");
                printf("PUSHFRAME\n");

                printf("JUMP vardef$%s\n", node->first->data.id->data);
                printf("LABEL vardef$%s$back\n", node->first->data.id->data);

                var_def = false;
                generate(node->second);

                in_func = true;
                printf("# Telo funkce\n");
                generate(node->third);
                generate(node->fourth);

                // sem skoci vsechny returny funkce
                printf("# Return funkce\n");
                printf("LABEL return$%s\n", node->first->data.id->data);

                // ulozi return hodnotu do globalni promenne
                printf("POPS GF@return_value\n");
                // nebo necham tu hodnotu na zasobniku? -> tj nebudu ji popovat

                printf("POPFRAME\n");
                printf("RETURN\n\n");
                printf("EXIT int@0\n");
                printf("LABEL error_exit\n");
                printf("EXIT int@99\n");
                // --konec funkce--

                printf("# Definice promennych\n");
                printf("LABEL vardef$%s\n", node->first->data.id->data);
                // define all params
                var_def = true;
                generate(node->second);

                printf("JUMP vardef$%s$back\n", node->first->data.id->data);

                printf("LABEL skip$%s", node->first->data.id->data);
                in_func = false;
            }
            break;

        case ParamsDefine_N:
            if (var_def) {
                printf("DEFVAR LF@%s\n", node->first->data.id->data);
            } else {
                printf("POPS LF@%s\n", node->first->data.id->data);
            }
            generate(node->second);
            generate(node->third);
            break;

        case ParamsDefineNext_N:
            if (var_def) {
                printf("DEFVAR LF@%s\n", node->first->data.id->data);
            } else {
                printf("POPS LF@%s\n", node->first->data.id->data);
            }
        //printf("DEFVAR LF@%s\n", node->first->data.id->data);
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
            switch (node->third->type) {
                case Id_N:
                    break;
                case FuncCall_N:
                    break;
                case Expression_N:
                    break;
                case Str_N:
                    break;
                default:
                    // chyba
                    printf("JUMP error_exit\n");
                    break;
            }
            if (node->third->type == Expression_N) {
                printf("DEFVAR LF@%s ", node->first->data.id->data);
                generate_expr(node->third, node->third->data.data_type);
                printf("POPS LF@%s\n", node->first->data.id->data);
            } else {
                printf("DEFVAR LF@%s\n", node->first->data.id->data);
                generate(node->third);
            }


        // if node->third == EXPRESSION -> generate_expr
            if (node->third->type == Expression_N) {
                printf("DEFVAR LF@%s ", node->first->data.id->data);
                generate_expr(node->third, node->third->data.data_type);
                printf("POPS LF@%s\n", node->first->data.id->data);
            } else {
                printf("DEFVAR LF@%s\n", node->first->data.id->data);
                generate(node->third);
            }
        /*
        printf("DEFVAR LF@%s\n", node->first->data.id->data);
    //printf("MOVE LF@%s %s@%s", node->first->data.id->data, data_type(node->second->data.data_type), node->third->data.id->data);
        printf("MOVE LF@%s ", node->first->data.id->data);
        */
            break;

        case VariableAssign_N:
            switch (node->third->type) {
                case Id_N:
                    break;
                case FuncCall_N:
                    break;
                case Expression_N:
                    break;
                case Str_N:
                    break;
                default:
                    // chyba
                    printf("JUMP error_exit\n");
                    break;
            }
            if (node->second->type == Expression_N) {
                generate_expr(node->second, node->second->data.data_type);
                printf("POPS LF@%s\n", node->first->data.id->data);
            } else {
                printf("MOVE LF@%s ", node->first->data.id->data);
                generate(node->second);
            }
            break;

        case FuncCall_N:
            generate(node->second);
            printf("CALL $%s\n", node->first->data.id->data);
            break;

        // pushuji parametry od konce!
        // aby se to lip pak parovalo s parametry funkce
        case Params_N:
            switch (node->third->type) {
                case Id_N:
                    break;
                case FuncCall_N:
                    break;
                case Expression_N:
                    break;
                case Str_N:
                    break;
                default:
                    // chyba
                    printf("JUMP error_exit\n");
                    break;
            }
        // pushs parametr
            generate(node->second);
            if (node->first->type == Expression_N) {
                generate_expr(node->first, node->first->data.data_type);
            } else {
                printf("PUSHS %s@%s\n", data_type(node->first), node->first->data.id->data);
            }
            break;

        case ParamsNext_N:
            switch (node->third->type) {
                case Id_N:
                    break;
                case FuncCall_N:
                    break;
                case Expression_N:
                    break;
                case Str_N:
                    break;
                default:
                    // chyba
                    printf("JUMP error_exit\n");
                    break;
            }
            generate(node->second);
            if (node->first->type == Expression_N) {
                generate_expr(node->first, node->first->data.data_type);
            } else {
                printf("PUSHS %s@%s\n", data_type(node->first), node->first->data.id->data);
            }
        //printf("PUSHS %s@%s\n", data_type(node->first->data.id->data), node->first->data.id->data);
            break;

        case If_N:
            if_counter++;
        // if bez pipes if () |neco| ..
            if (node->second->data.has_not_null_id == false) {
                generate_expr(node->first, node->first->data.data_type);
                printf("POPS GF@exp_return\n");
                printf("JUMPIFEQ $if$%d$else GF@exp_return bool@false\n", if_counter);
                generate(node->second);
                printf("JUMP $if$%d$end\n", if_counter);
                printf("LABEL $if$%d$else\n", if_counter);
                generate(node->third);
                printf("LABEL $if$%d$end\n", if_counter);
            } else {
                // TODO: dodelat if s pipes |neco|
                // if (vyraz s null) |id bez null| -> pokud vyraz s null neni null -> prevedu do id bez null s odpovidajici hodnotou
                generate_expr(node->first, node->first->data.data_type);
                printf("POPS GF@exp_return\n");
                printf("JUMPIFEQ $if$%d$else GF@exp_return bool@false\n", if_counter);
                generate(node->second);
                printf("LABEL $if$%d$else\n", if_counter);
                generate(node->third);
            }

            break;

        case While_N:
            while_counter++;
        // while bez pipes while () |neco| ..
            if (node->second->data.has_not_null_id == false) {
                printf("LABEL while$%d\n", while_counter);
                generate_expr(node->first, node->first->data.data_type);
                printf("POPS GF@exp_return\n");
                printf("JUMPIFEQ $while$%d$end GF@exp_return bool@false\n", while_counter);
                generate(node->second);
                printf("JUMP while$%d\n", while_counter);
                printf("LABEL $while$%d$end\n", while_counter);
            } else {
                // TODO: dodelat while s pipes |neco|
            }
            break;

        case VoidCall_N:
            generate(node->second);
            printf("CALL $%s\n", node->first->data.id->data);
            break;

        case ReturnStatement_N:
            switch (node->third->type) {
                case Id_N:
                    break;
                case FuncCall_N:
                    break;
                case Expression_N:
                    break;
                case Str_N:
                    break;
                default:
                    // chyba
                    printf("JUMP error_exit\n");
                    break;
            }
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
            // TODO:
            // if .data.has_not_null_id == true -> true je s pipema
            // udelam promenou tomu pipam
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
void generate_expr(Node *node, VarType expr_type) {
    // pro realcni udelat zvlast protoze se to chova jinak nez int
    // kdyz to je string tak to je ve string.id
    // funkce jsou jinak function_call_N

    if (node == NULL) return; // break

    // rozlisit realcni a int
    // bool -> ANDS, ORS, NOTS


    switch (node->type) {
        case Id_N:
            // TODO: nejak tam tu promennou dat misto "symbol"
            printf("PUSHS LF@%s\n", node->data.id->data);
        // if null push null nill
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
            printf("GTS\nNOTS\n");
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
            printf("LTS\nNOTS\n");
            break;

        case Eq_N:
            generate_expr(node->first, expr_type);
            generate_expr(node->second, expr_type);
        // do something
            printf("EQS\n");
            break;

        case NotEq_N:
            generate_expr(node->first, expr_type);
            generate_expr(node->second, expr_type);
        // do something
            printf("EQS\nNOTS\n");
            break;

        case Plus_N:
            generate_expr(node->first, expr_type);
            generate_expr(node->second, expr_type);
            printf("ADDS\n");
            break;

        case Minus_N:
            generate_expr(node->first, expr_type);
            generate_expr(node->second, expr_type);
            printf("SUBS\n");
            break;

        case Times_N:
            generate_expr(node->first, expr_type);
            generate_expr(node->second, expr_type);
            printf("MULS\n");
            break;

        case Divide_N:
            generate_expr(node->first, expr_type);
            generate_expr(node->second, expr_type);

        // TODO: nejak zkontrolovat jestli pouzit DIV nebo IDIV
        // if % then IDIVS else DIVS
        // div float, idiv int
            if (expr_type == Float_N || expr_type == Float_N) {
                printf("DIVS\n");
            } else {
                printf("IDIVS\n");
            }
            break;

        case FuncCall_N:
            generate(node->first);
            break;

        default:
            ThrowError(99);
            break;
    }

    return;
}

char *data_type(Node* node) {
    switch (node->type) {
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
            // error?
            return "";
            break;
    }
}

// asi jeste vyuziju, aby to bylo na jednom miste (pujde to?)
// func with switch case to determine "rhs"
NodeType get_rhs(NodeType type) {
    switch (type) {
        case Int_N:
            return Int_N;
            break;
        case Float_N:
            return Float_N;
            break;
        case Id_N:
            return Id_N;
            break;
        case FuncCall_N:
            return FuncCall_N;
            break;
        default:
            return Int_N;
            break;
    }
}
