/** @file generator.c
* @author Daniel Geier (xgeierd00)
* @author Patrik Mokrusa (xmokrup00)
*
* IFJ24
*
* @brief Implementace generatoru
*/

#include "generator.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "error.h"
#include "symtable.h"
#include "parser.h"

void generate(Node *node) {
    if (node == NULL) return;

    // pocet if statements
    static int if_counter = 0;
    // pocet while statements
    static int while_counter = 0;

    // pomocna promenna jestli definujeme promenne
    static bool var_def = false;
    // pomocna promenna jestli se nachazime uvnitr while
    static bool in_while = false;

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
            if (strcmp(node->first->data.id->data, "main") == 0) {
                printf("LABEL $$main\n");
                printf("CREATEFRAME\n");
                printf("PUSHFRAME\n");

                // globalni promena pro vraceni hodnot z funkci
                printf("DEFVAR GF@value_return\n");
                // globalni promenna pro vraceni true/false z expresion
                printf("DEFVAR GF@exp_return\n");
                // globalni promenna pro zahozeni hodnoty
                printf("DEFVAR GF@devnull\n");
                // generovani vestavenych funkci
                generate_builtin();

                generate(node->second);
                generate(node->third);
                generate(node->fourth);

                printf("POPFRAME\n");
                printf("EXIT int@0\n");

                // division by zero error exit 57
                printf("LABEL division_by_zero$\n");
                printf("POPFRAME\n");
                printf("EXIT int@57\n");

                // index out of bounds error exit 58
                printf("LABEL $index_out_of_bounds$\n");
                printf("POPFRAME\n");
                printf("EXIT int@58\n");

                printf("LABEL error_exit\n");
                printf("POPFRAME\n");
                printf("EXIT int@9\n");
            } else {
                printf("JUMP skip$%s\n", node->first->data.id->data);
                printf("LABEL $%s\n", node->first->data.id->data);
                printf("CREATEFRAME\n");
                printf("PUSHFRAME\n");

                printf("JUMP vardef$%s\n", node->first->data.id->data);
                printf("LABEL vardef$%s$back\n", node->first->data.id->data);

                var_def = false;
                generate(node->second);

                printf("# Telo funkce\n");
                generate(node->third);
                generate(node->fourth);

                printf("# Return funkce\n");
                printf("POPFRAME\n");
                printf("RETURN\n\n");

                printf("# Definice promennych\n");
                printf("LABEL vardef$%s\n", node->first->data.id->data);

                // definice promennych
                var_def = true;
                generate(node->second);
                var_def = false;

                printf("JUMP vardef$%s$back\n", node->first->data.id->data);

                printf("LABEL skip$%s\n", node->first->data.id->data);
            }
            break;

        case ReturnStatement_N:
            if (node->first == NULL) {
                printf("POPFRAME\n");
                printf("RETURN\n\n");

                return;
            }

            switch (node->first->type) {
                case Id_N:
                    printf("MOVE GF@value_return LF@%s$\n", node->first->data.id->data);
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
                    char *str = escape_string(node->first->data.str->data);
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
        case ParamsDefineNext_N:
            if (var_def) {
                printf("DEFVAR LF@%s$\n", node->first->data.id->data);
            } else {
                printf("POPS LF@%s$\n", node->first->data.id->data);
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
            if (!in_while) {
                printf("DEFVAR LF@%s$\n", node->first->data.id->data);
            }

            switch (node->third->type) {
                case Id_N:
                    if (strcmp(node->third->data.id->data, "null") == 0) {
                        printf("MOVE LF@%s$ nil@nil\n", node->first->data.id->data);
                    } else {
                        printf("MOVE LF@%s$ LF@%s$\n", node->first->data.id->data, node->second->data.id->data);
                    }
                    break;
                case FuncCall_N:
                    generate(node->third);
                    printf("MOVE LF@%s$ GF@value_return\n", node->first->data.id->data);
                    break;
                case Expression_N:
                    generate_expr(node->third, node->third->data.data_type);
                    printf("POPS LF@%s$\n", node->first->data.id->data);
                    break;
                case Str_N:
                    char *str = escape_string(node->third->data.str->data);
                    printf("MOVE LF@%s$ string@%s\n", node->first->data.id->data, str);
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
                        if (strcmp(node->second->data.id->data, "null") == 0) {
                            printf("MOVE LF@%s$ nil@nil\n", node->first->data.id->data);
                        } else {
                            printf("MOVE LF@%s$ LF@%s$\n", node->first->data.id->data, node->second->data.id->data);
                        }
                        break;
                    case FuncCall_N:
                        generate(node->second);
                        printf("MOVE LF@%s$ GF@value_return\n", node->first->data.id->data);
                        break;
                    case Expression_N:
                        generate_expr(node->second, node->second->data.data_type);
                        printf("POPS LF@%s$\n", node->first->data.id->data);
                        break;
                    case Str_N:
                        char *str = escape_string(node->second->data.str->data);
                        printf("MOVE LF@%s$ string@%s\n", node->first->data.id->data, str);
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
            convert_builtin(node->first->data.id->data);
            break;

        case Params_N:
        case ParamsNext_N:
            // parametry se pushuji od konce
            generate(node->second);
            switch (node->first->type) {
                case Id_N:
                    generate_expr(node->first, node->first->data.data_type);
                    break;
                case FuncCall_N:
                    generate_expr(node->first, node->first->data.data_type);
                    break;
                case Expression_N:
                    generate_expr(node->first, node->first->data.data_type);
                    break;
                case Str_N:
                    char *str = escape_string(node->first->data.str->data);
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
            int if_counter_in = if_counter;
        // if bez |pipes|
            if (node->data.has_not_null_id == false) {
                generate_expr(node->first, node->first->data.data_type);
                printf("POPS GF@exp_return\n");
                printf("JUMPIFEQ $if$%d$else GF@exp_return bool@false\n", if_counter_in);
                generate(node->second);
                printf("JUMP $if$%d$end\n", if_counter_in);
                printf("LABEL $if$%d$else\n", if_counter_in);
                generate(node->third);
                printf("LABEL $if$%d$end\n", if_counter_in);
            } else {
                printf("JUMPIFEQ $if$%d$else LF@%s$ nil@nil\n", if_counter_in, node->first->data.id->data);
                printf("DEFVAR LF@%s$\n", node->second->data.id->data);
                printf("MOVE LF@%s$ LF@%s$\n", node->second->data.id->data, node->first->data.id->data);
                generate(node->third);
                printf("JUMP $if$%d$end\n", if_counter_in);
                printf("LABEL $if$%d$else\n", if_counter_in);
                generate(node->fourth);
                printf("LABEL $if$%d$end\n", if_counter_in);
            }

            break;

        case While_N:
            while_counter++;
            int while_counter_in = while_counter;

        // zadefinovat si promenne mimo while (redefinice)
            if (node->data.has_not_null_id == false) {
                Node *tmp = node->second;
                while (tmp != NULL) {
                    if (tmp->first->first->type == VariableDefine_N) {
                        printf("DEFVAR LF@%s$\n", tmp->first->first->first->data.id->data);
                    }
                    if ((tmp->first->first->type == If_N) && (tmp->first->first->data.has_not_null_id == true)) {
                        printf("DEFVAR LF@%s$\n", tmp->first->first->second->data.id->data);
                    }
                    tmp = tmp->second;
                }

                in_while = true;
                printf("LABEL while$%d\n", while_counter_in);
                generate_expr(node->first, node->first->data.data_type);
                printf("POPS GF@exp_return\n");
                printf("JUMPIFEQ $while$%d$end GF@exp_return bool@false\n", while_counter_in);
                generate(node->second);
                printf("JUMP while$%d\n", while_counter_in);
                printf("LABEL $while$%d$end\n", while_counter_in);
            } else {
                Node *tmp = node->third;
                while (tmp != NULL) {
                    if (tmp->first->first->type == VariableDefine_N) {
                        printf("DEFVAR LF@%s$\n", tmp->first->first->first->data.id->data);
                    }
                    if ((tmp->first->first->type == If_N) && (tmp->first->first->data.has_not_null_id == true)) {
                        printf("DEFVAR LF@%s$\n", tmp->first->first->second->data.id->data);
                    }
                    tmp = tmp->second;
                }
                in_while = true;

                printf("DEFVAR LF@%s$\n", node->second->data.id->data);
                printf("LABEL while$%d\n", while_counter_in);
                printf("JUMPIFEQ $while$%d$end LF@%s$ nil@nil\n", while_counter_in, node->first->data.id->data);
                printf("MOVE LF@%s$ LF@%s$\n", node->second->data.id->data, node->first->data.id->data);
                generate(node->third);
                printf("JUMP while$%d\n", while_counter_in);
                printf("LABEL $while$%d$end\n", while_counter_in);
            }
            in_while = false;
            break;

        case VoidCall_N:
            generate(node->second);
            convert_builtin(node->first->data.id->data);
            break;

        default:
            generate(node->first);
            generate(node->second);
            generate(node->third);
            generate(node->fourth);
            break;
    }
}

void generate_expr(Node *node, VarType expr_type) {
    // pro realcni udelat zvlast protoze se to chova jinak nez int

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
            if (strcmp(node->data.id->data, "null") == 0) {
                printf("PUSH nil@nil\n");
            } else {
                printf("PUSHS LF@%s$\n", node->data.id->data);
            }
        // if null push null nill
            break;

        case Float_N:
            printf("PUSHS float@%a\n", node->data.flt);
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

            printf("POPS GF@exp_return\n");
            printf("JUMPIFEQ division_by_zero$ GF@exp_return int@0\n");
            printf("PUSHS GF@exp_return\n");


            if (expr_type == TYPE_FLOAT) {
                printf("DIVS\n");
            } else {
                printf("IDIVS\n");
            }
            break;

        case FuncCall_N:
            generate(node->second);
            convert_builtin(node->first->data.id->data);
            printf("PUSHS GF@value_return\n");
            break;

        default:
            ThrowError(99);
            break;
    }

    return;
}

// replace the "." in ifj builtin funcs for "_"
// will probably remove in future, do this replacement in parser
void convert_builtin(const char *str) {
    if (strcmp(str, "ifj.write") == 0) {
        printf("CALL $ifj_write\n");
    } else if (strcmp(str, "ifj.string") == 0) {
        printf("CALL $ifj_string\n");
    } else if (strcmp(str, "ifj.readi32") == 0) {
        printf("CALL $ifj_readi32\n");
    } else if (strcmp(str, "ifj.readf64") == 0) {
        printf("CALL $ifj_readf64\n");
    } else if (strcmp(str, "ifj.readstr") == 0) {
        printf("CALL $ifj_readstr\n");
    } else if (strcmp(str, "ifj.i2f") == 0) {
        printf("CALL $ifj_i2f\n");
    } else if (strcmp(str, "ifj.f2i") == 0) {
        printf("CALL $ifj_f2i\n");
    } else if (strcmp(str, "ifj.concat") == 0) {
        printf("CALL $ifj_concat\n");
    } else if (strcmp(str, "ifj.strcmp") == 0) {
        printf("CALL $ifj_strcmp\n");
    } else if (strcmp(str, "ifj.length") == 0) {
        printf("CALL $ifj_length\n");
    } else if (strcmp(str, "ifj.substring") == 0) {
        printf("CALL $ifj_substring\n");
    } else if (strcmp(str, "ifj.ord") == 0) {
        printf("CALL $ifj_ord\n");
    } else if (strcmp(str, "ifj.chr") == 0) {
        printf("CALL $ifj_chr\n");
    } else {
        printf("CALL $%s\n", str);
    }
    return;
}

// prevede string na escape sekvenci
char *escape_string(const char *str) {
    // sekvence ma 4 znaky
    char *new_str = malloc(strlen(str) * 4 + 1);
    if (new_str == NULL) {
        ThrowError(99);
    }
    // pointer na zacatek stringu
    char *ret_str = new_str;

    char n;
    while ((n = *(str++)) != '\0') {
        // ascii hodnoty kontrolnich znaku, mezery, #, zpetne lomitko
        if (n <= 32 || n == 35 || n == 92) {
            sprintf(new_str, "\\%03u", n);
            // posunuti na dalsi sekvenci
            new_str += 4;
        } else {
            // jinak zkopirujeme znak
            *new_str++ = n;
        }
    }
    *new_str = '\0';
    return ret_str;
}

void generate_builtin() {
    printf("\n# Builtin functions\n");
    printf("JUMP $skip_builtin\n");

    // write
    //printf("JUMP $skip_write\n");
    printf("LABEL $ifj_write\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@tmp\n");
    printf("POPS LF@tmp\n");
    printf("WRITE LF@tmp\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_write\n");

    // read functions
    // read int
    //printf("JUMP $skip_readi32\n");
    printf("LABEL $ifj_readi32\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("READ GF@value_return int\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_readi32\n");

    // read float
    //printf("JUMP $skip_readf64\n");
    printf("LABEL $ifj_readf64\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("READ GF@value_return float\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_readf64\n");

    // read string
    //printf("JUMP $skip_readstr\n");
    printf("LABEL $ifj_readstr\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("READ GF@value_return string\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_readstr\n");

    /*
    // read bool
    printf("JUMP $skip_readbool\n");
    printf("LABEL $ifj_read_bool\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("READ GF@value_return bool\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $skip_readbool\n");
    */

    // work with strings
    // string
    //printf("JUMP $skip_string\n");
    printf("LABEL $ifj_string\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("POPS GF@value_return\n");
    // prevest string na escape sequence
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_string\n");

    // concat
    //printf("JUMP $skip_concat\n");
    printf("LABEL $ifj_concat\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@tmp1\n");
    printf("DEFVAR LF@tmp2\n");
    printf("POPS LF@tmp1\n");
    printf("POPS LF@tmp2\n");
    printf("CONCAT GF@value_return LF@tmp1 LF@tmp2\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_concat\n");

    // strcmp
    //printf("JUMP $skip_strcmp\n");
    printf("LABEL $ifj_strcmp\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@str1\n");
    printf("DEFVAR LF@str2\n");
    printf("POPS LF@str1\n");
    printf("POPS LF@str2\n");

    printf("DEFVAR LF@len1\n");
    printf("DEFVAR LF@len2\n");
    printf("STRLEN LF@len1 LF@str1\n");
    printf("STRLEN LF@len2 LF@str2\n");

    printf("DEFVAR LF@tmp1\n");
    printf("EQ LF@tmp1 LF@len1 LF@len2\n");
    printf("JUMPIFEQ $ifj_strcmp$diff LF@tmp1 bool@false\n");

    // stejne delky
    printf("DEFVAR LF@char1\n");
    printf("DEFVAR LF@char2\n");
    printf("DEFVAR LF@ord1\n");
    printf("DEFVAR LF@ord2\n");
    printf("DEFVAR LF@index\n");
    printf("DEFVAR LF@tmp2\n");
    printf("DEFVAR LF@len-1\n");
    printf("SUB LF@len-1 LF@len1 int@1\n");

    printf("MOVE LF@index int@0\n");
    printf("LABEL $ifj_strcmp$loop\n");
    // jestli je index roven delce tak jsme mimo retezec vracim chybu 58
    printf("EQ LF@tmp1 LF@index LF@len1\n");
    printf("JUMPIFEQ $index_out_of_bounds$ LF@tmp1 bool@true\n");
    // prevedeni jednoho znaku retezce na ord hodnotu
    printf("STRI2INT LF@ord1 LF@str1 LF@index\n");
    printf("STRI2INT LF@ord2 LF@str2 LF@index\n");
    // porovnani dvou ord hodnot
    printf("SUB LF@tmp2 LF@ord1 LF@ord2\n");
    // pokud jsou hodnoty rozdilne tak vracim -1 nebo 1
    printf("LT LF@tmp1 LF@tmp2 int@0\n");
    printf("JUMPIFEQ $ifj_strcmp$smallstr1 LF@tmp1 bool@true\n");
    printf("GT LF@tmp1 LF@tmp2 int@0\n");
    printf("JUMPIFEQ $ifj_strcmp$smallstr2 LF@tmp1 bool@true\n");
    // pokud jsou hodnoty stejne tak pokracuji na dalsi znak
    printf("EQ LF@tmp1 LF@tmp2 int@0\n");
    printf("EQ LF@tmp2 LF@index LF@len-1\n");
    printf("EQ LF@tmp2 LF@tmp1 LF@tmp2\n");
    printf("JUMPIFEQ $ifj_strcmp$same LF@tmp2 bool@true\n");
    printf("ADD LF@index LF@index int@1\n");
    printf("JUMP $ifj_strcmp$loop\n");

    // rozdilne delky
    printf("LABEL $ifj_strcmp$diff\n");
    printf("LT LF@tmp1 LF@len1 LF@len2\n");
    printf("JUMPIFEQ $ifj_strcmp$smallstr1 LF@tmp1 bool@true\n");
    printf("JUMP $ifj_strcmp$smallstr2\n");

    // str1 > str2
    printf("LABEL $ifj_strcmp$smallstr2\n");
    printf("MOVE GF@value_return int@1\n");
    printf("JUMP $ifj_strcmp$end\n");

    // str1 < str2
    printf("LABEL $ifj_strcmp$smallstr1\n");
    printf("MOVE GF@value_return int@-1\n");
    printf("JUMP $ifj_strcmp$end\n");

    // str1 == str2
    printf("LABEL $ifj_strcmp$same\n");
    printf("MOVE GF@value_return int@0\n");

    printf("LABEL $ifj_strcmp$end\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_strcmp\n");

    // length
    //printf("JUMP $skip_length\n");
    printf("LABEL $ifj_length\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@tmp\n");
    printf("POPS LF@tmp\n");
    printf("STRLEN GF@value_return LF@tmp\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_length\n");

    // substr
    //printf("JUMP $skip_substring\n");
    printf("LABEL $ifj_substring\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@str\n");
    printf("DEFVAR LF@index1\n");
    printf("DEFVAR LF@index2\n");
    printf("DEFVAR LF@len\n");
    printf("DEFVAR LF@tmp\n");
    printf("DEFVAR LF@strRet\n");

    printf("POPS LF@str\n");
    printf("POPS LF@index1\n");
    printf("POPS LF@index2\n");

    printf("STRLEN LF@len LF@str\n");

    // i < 0
    printf("LT LF@tmp LF@index1 int@0\n");
    printf("JUMPIFEQ $return_nil LF@tmp bool@true\n");

    // j < 0
    printf("LT LF@tmp LF@index2 int@0\n");
    printf("JUMPIFEQ $return_nil LF@tmp bool@true\n");

    // i > j
    printf("GT LF@tmp LF@index1 LF@index2\n");
    printf("JUMPIFEQ $return_nil LF@tmp bool@true\n");

    // i >= len
    printf("LT LF@tmp LF@index1 LF@len\n");
    printf("JUMPIFEQ $return_nil LF@tmp bool@false\n");

    // j > len
    printf("GT LF@tmp LF@index2 LF@len\n");
    printf("JUMPIFEQ $return_nil LF@tmp bool@true\n");

    printf("MOVE LF@strRet string@\n");

    // prochazeni retezce
    printf("LABEL $substr$loop\n");
    printf("JUMPIFEQ $substr$end LF@index1 LF@index2\n");
    printf("GETCHAR LF@tmp LF@str LF@index1\n");
    printf("CONCAT LF@strRet LF@strRet LF@tmp\n");
    printf("ADD LF@index1 LF@index1 int@1\n");
    printf("JUMP $substr$loop\n");

    printf("LABEL $return_nil\n");
    printf("MOVE GF@value_return nil@nil\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $substr$end\n");
    printf("MOVE GF@value_return LF@strRet\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_substring\n");

    // ord
    //printf("JUMP $skip_ord\n");
    printf("LABEL $ifj_ord\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@str\n");
    printf("DEFVAR LF@index\n");

    printf("POPS LF@str\n");
    printf("POPS LF@index\n");
    printf("JUMPIFEQ $ifj_ord$null LF@str nil@nil\n");
    printf("DEFVAR LF@tmp\n");
    printf("DEFVAR LF@len\n");
    printf("STRLEN LF@len LF@str\n");
    printf("JUMPIFEQ $ifj_ord$null LF@index int@0\n");
    printf("LT LF@tmp LF@index LF@len\n");
    printf("JUMPIFEQ $ifj_ord$null LF@tmp bool@false\n");

    printf("STRI2INT GF@value_return LF@str LF@index\n");
    printf("POPFRAME\n");
    printf("RETURN\n");

    printf("LABEL $ifj_ord$null\n");
    printf("MOVE GF@value_return int@0\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_ord\n");

    // chr
    //printf("JUMP $skip_chr\n");
    printf("LABEL $ifj_chr\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("INT2CHARS\n");
    printf("POPS GF@value_return\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_chr\n");

    // convert functions
    // int to float
    //printf("JUMP $skip_i2f\n");
    printf("LABEL $ifj_i2f\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("INT2FLOATS\n");
    printf("POPS GF@value_return\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_i2f\n");

    // float to int
    //printf("JUMP $skip_f2i\n");
    printf("LABEL $ifj_f2i\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");
    printf("FLOAT2INTS\n");
    printf("POPS GF@value_return\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    //printf("LABEL $skip_f2i\n");

    printf("LABEL $skip_builtin\n\n");
}
