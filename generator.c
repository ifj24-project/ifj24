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
#include <stdlib.h>

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
    // static int void_call_counter = 0;

    switch (node->type) {
        case Start_N:
            generate(node->first);
            generate(node->second);

            break;

        case ProgramProlog_N:
            // Kod v jazyce mezikodu IFJ24 zacina radkem
            printf(".IFJcode24\n\n");
            printf("JUMP $$main\n");
            break;

        case Program_N:
            generate(node->first);
            generate(node->second);

            break;


        case DataType_N:
            break;

        case FuncDefine_N:
            in_func = true;
            if (strcmp(node->first->data.id->data, "main") == 0) {
                printf("LABEL $$main\n");
                printf("CREATEFRAME\n");
                printf("PUSHFRAME\n");

                // globalni promena pro vraceni hodnot z funkci
                printf("DEFVAR GF@value_return\n");
                // globalni promenna pro vraceni true/false z expresion
                printf("DEFVAR GF@exp_return\n");
                // globalni promenne pro expresions lhs a rhs
                printf("DEFVAR GF@lhs\n");
                printf("DEFVAR GF@rhs\n");
                printf("DEFVAR GF@devnull\n");
                generate_builtin();

                generate(node->second);
                generate(node->third);
                generate(node->fourth);

                printf("POPFRAME\n");
                printf("EXIT int@0\n");
                printf("LABEL error_exit\n");
                printf("EXIT int@99\n");
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
                //printf("POPS GF@return_value\n");
                // nebo necham tu hodnotu na zasobniku? -> tj nebudu ji popovat

                printf("POPFRAME\n");
                printf("RETURN\n\n");
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

        case ReturnStatement_N:
            switch (node->first->type) {
                case Id_N:
                    printf("MOVE GF@value_return LF@%s\n", node->first->data.id->data);
                    break;
                case FuncCall_N:
                    generate(node->first);
                // hodnota je ulozena z predchoziho volani v GF@value_return
                    break;
                case Expression_N:
                    generate_expr(node->first, node->first->data.data_type);
                    printf("POPS GF@value_return\n");
                    break;
                case Str_N:
                    char* str = escape_string(node->first->data.str->data);
                    printf("MOVE GF@value_return string@%s\n", str);
                    free(str);
                    break;
                default:
                    // chyba
                    printf("JUMP error_exit\n");
                    break;
            }
            generate(node->first);
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
            printf("DEFVAR LF@%s\n", node->first->data.id->data);
            switch (node->third->type) {
                case Id_N:
                    printf("MOVE LF@%s LF@%s\n", node->first->data.id->data, node->third->data.id->data);
                    break;
                case FuncCall_N:
                    generate(node->third);
                    printf("MOVE LF@%s GF@value_return\n", node->first->data.id->data);
                    break;
                case Expression_N:
                    generate_expr(node->third, node->third->data.data_type);
                    printf("POPS LF@%s\n", node->first->data.id->data);
                    break;
                case Str_N:
                    char* str = escape_string(node->third->data.str->data);
                    printf("MOVE LF@%s string@%s\n", node->first->data.id->data, str);
                    free(str);
                    break;
                default:
                    // chyba
                    printf("JUMP error_exit\n");
                    break;
            }
            break;

        case VariableAssign_N:
            if (node->first->type == Underscore_N) {
                switch (node->second->type) {
                    case Id_N:
                    case Str_N:
                        break;
                    case FuncCall_N:
                        generate(node->second);
                        printf("MOVE GF@devnull GF@value_return\n");
                        break;
                    case Expression_N:
                        generate_expr(node->second, node->second->data.data_type);
                        printf("POPS GF@devnull\n");
                        break;
                    default:
                        // chyba
                        printf("JUMP error_exit\n");
                        break;
                }
            } else {
                switch (node->second->type) {
                    case Id_N:
                        printf("MOVE LF@%s LF@%s\n", node->first->data.id->data, node->second->data.id->data);
                        break;
                    case FuncCall_N:
                        generate(node->second);
                        printf("MOVE LF@%s GF@value_return\n", node->first->data.id->data);
                        break;
                    case Expression_N:
                        generate_expr(node->second, node->second->data.data_type);
                        printf("POPS LF@%s\n", node->first->data.id->data);
                        break;
                    case Str_N:
                        char* str = escape_string(node->second->data.str->data);
                        printf("MOVE LF@%s string@%s\n", node->first->data.id->data, str);
                        free(str);
                        break;
                    default:
                        // chyba
                        printf("JUMP error_exit\n");
                        break;
                }
            }
            break;

        case FuncCall_N:
            generate(node->second);
            if(strcmp(node->first->data.id->data, "ifj.write") == 0) {
                printf("CALL $ifj_write\n");
            } else if (strcmp(node->first->data.id->data, "ifj.string") == 0) {
				printf("CALL $ifj_string\n");
			} else {
				printf("CALL $%s\n", node->first->data.id->data);
			}
            break;

        // pushuji parametry od konce!
        case Params_N:
        case ParamsNext_N:
            generate(node->second);
            switch (node->first->type) {
                case Id_N:
                    printf("PUSHS LF@%s\n", node->first->data.id->data);
                    break;
                case FuncCall_N:
                    generate(node->first);
                    printf("PUSHS GF@value_return\n");
                    break;
                case Expression_N:
                    generate_expr(node->first, node->first->data.data_type);
                    break;
                case Str_N:
                    char* str = escape_string(node->first->data.str->data);
                    printf("PUSHS string@%s\n", str);
                    free(str);

                    break;
                default:
                    // chyba
                    printf("JUMP error_exit\n");
                    break;
            }
            break;

        case If_N:
            if_counter++;
        // if bez pipes if () |neco| ..
            if (node->data.has_not_null_id == false) {
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
                printf("JUMPIFEQ $if$%d$else LF@%s nil@nil\n", if_counter, node->first->data.id->data);
                printf("DEFVAR LF@%s\n", node->second->data.id->data);
                printf("MOVE LF@%s LF@%s\n", node->first->data.id->data, node->second->data.id->data);
                generate(node->third);
                printf("JUMP $if$%d$end\n", if_counter);
                printf("LABEL $if$%d$else\n", if_counter);
                generate(node->fourth);
                printf("LABEL $if$%d$end\n", if_counter);
            }

            break;

        case While_N:
            while_counter++;
        // while bez pipes while () |neco| ..
            if (node->data.has_not_null_id == false) {
                printf("LABEL while$%d\n", while_counter);
                generate_expr(node->first, node->first->data.data_type);
                printf("POPS GF@exp_return\n");
                printf("JUMPIFEQ $while$%d$end GF@exp_return bool@false\n", while_counter);
                generate(node->second);
                printf("JUMP while$%d\n", while_counter);
                printf("LABEL $while$%d$end\n", while_counter);
            } else {
                // TODO: destruktor
                printf("DEFVAR LF@%s\n", node->second->data.id->data);
                printf("LABEL while$%d\n", while_counter);
                printf("JUMPIFEQ $while$%d$end LF@%s nil@nil\n", while_counter, node->first->data.id->data);
                printf("MOVE LF@%s LF@%s\n", node->second->data.id->data, node->first->data.id->data);
                generate(node->third);
                printf("JUMP while$%d\n", while_counter);
                printf("LABEL $while$%d$end\n", while_counter);
            }
            break;

        case VoidCall_N:
            generate(node->second);
            if(strcmp(node->first->data.id->data, "ifj.write") == 0) {
                printf("CALL $ifj_write\n");
            } else {
                printf("CALL $%s\n", node->first->data.id->data);
            }
            break;


        default:
            generate(node->first);
            generate(node->second);
            generate(node->third);
            generate(node->fourth);
            break;
    }
    // Exit success
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
    /*
    if (expr_type == DT_BOOL) {
        switch (node->type) {
            // ANDS, ORS, NOTS
        }
    }
    */


    switch (node->type) {
        case Expression_N:
            generate_expr(node->first, expr_type);
            break;

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
        case Underscore_N:
            break;

        case Lesser_N:
            generate_expr(node->first, node->data.bool_val.left);
            generate_expr(node->second, node->data.bool_val.right);
            printf("LTS\n");
            break;

        case LesserEq_N:
            generate_expr(node->first, node->data.bool_val.left);
            generate_expr(node->second, node->data.bool_val.right);
            printf("GTS\nNOTS\n");
            break;

        case Greater_N:
            generate_expr(node->first, node->data.bool_val.left);
            generate_expr(node->second, node->data.bool_val.right);
            printf("GTS\n");
            break;

        case GreaterEq_N:
            generate_expr(node->first, node->data.bool_val.left);
            generate_expr(node->second, node->data.bool_val.right);
        // do something
            printf("LTS\nNOTS\n");
            break;

        case Eq_N:
            generate_expr(node->first, node->data.bool_val.left);
            generate_expr(node->second, node->data.bool_val.right);
        // do something
            printf("EQS\n");
            break;

        case NotEq_N:
            generate_expr(node->first, node->data.bool_val.left);
            generate_expr(node->second, node->data.bool_val.right);
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

            if (expr_type == TYPE_FLOAT) {
                printf("DIVS\n");
            } else {
                printf("IDIVS\n");
            }
            break;

        case FuncCall_N:
            generate(node->first);
            printf("PUSH GF@return_value\n");
            break;

        default:
            ThrowError(99);
            break;
    }

    return;
}

char *data_type(int type) {
    switch (type) {
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
            return -1;
            break;
    }
}

// convert string to escape sequence
char* escape_string(const char *str) {
    // sequences are 4 characters long
    char *new_str = malloc(strlen(str) * 4 + 1);
    if (new_str == NULL) {
        ThrowError(99);
    }
    char *ret_str;
    ret_str = new_str;

    char n;
    while ((n = *(str++)) != '\0') {
        // ascii values of control chars, space, #, backslash
        if (n <= 32 || n == 35 || n == 92) {
            // escape sequence is in format

            sprintf(new_str, "\\%03u", n);
            // move to next sequence
            new_str += 4;
        } else {
            // move to next character
            *new_str++ = n;
        }
    }
    // appends null terminator
    *new_str = '\0';
    return ret_str;
    // TODO: free(new_str);
}

void generate_builtin() {

    // TODO
    // .string
    printf("JUMP $skip_string\n");
    printf("LABEL $ifj_string\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("POPS GF@value_return\n");
    // prevest string na escape sequence
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $skip_string\n");

    // write
    printf("JUMP $skip_write\n");
    printf("LABEL $ifj_write\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@_write\n");
    printf("POPS LF@_write\n");
    printf("WRITE LF@_write\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $skip_write\n");


    // read functions
    // read int
    printf("JUMP $skip_readi32\n");
    printf("LABEL $ifj_readi32\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("READ GF@rvalue_return int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $skip_readi32\n");

    // read float
    printf("JUMP $skip_readf64\n");
    printf("LABEL $ifj_readf64\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("READ GF@rvalue_return float\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $skip_readf64\n");

    // read string
    printf("JUMP $skip_readstr\n");
    printf("LABEL $ifj_readstr\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("READ GF@rvalue_return string\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $skip_readstr\n");

    // read bool
    printf("JUMP $skip_readbool\n");
    printf("LABEL $ifj_read_bool\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("READ GF@rvalue_return bool\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $skip_readbool\n");

    // concat
    printf("JUMP $skip_concat\n");
    printf("LABEL $ifj_concat\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $skip_concat\n");

    // strcmp
    printf("JUMP $skip_strcmp\n");
    printf("LABEL $ifj_strcmp\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $skip_strcmp\n");

    //

    // TODO: strlen, setchar, getchar, substr, ord, chr, nilcheck, length, all convert fncs, type
}
