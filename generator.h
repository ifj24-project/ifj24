/** @file generator.h
* @author Daniel Geier (xgeierd00)
*
* IFJ24
*
* @brief Deklarace funkci generatoru
*/

#ifndef GENERATOR_H
#define GENERATOR_H

#include "parser.h"
#include "symtable.h"

/**
 * @brief Funkce pro generovani kodu
 *
 * @param node ukazatel na uzel stromu
 */
void generate(Node* node);

/**
 * @brief Funkce pro generovani vyrazu
 *
 * @param node ukazatel na uzel stromu
 * @param expr_type typ vyrazu
 */
void generate_expr(Node* node, VarType expr_type);

/**
 * @brief Funkce pro prevedeni a zavolani vestavene funkce
 *
 * @param str nazev funkce
 */
void convert_builtin(const char* str);

/**
 * @brief Funkce pro prevedeni retezce na retezec v jazyce IFJ24
 *
 * @param str retezec
 * @return retezec v jazyce IFJ24
 */
char* escape_string(const char* str);

/**
 * @brief Funkce pro generovani vestavenych funkci
 */
void generate_builtin();

#endif //GENERATOR_H
