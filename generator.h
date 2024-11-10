/** @file generator.h
* @author Daniel Geier (xgeierd00)
*
* IFJ24
*
* @brief Deklarace funkci generatoru
*/

#ifndef GENERATOR_H
#define GENERATOR_H

#include "parser/parser.h"

/**
 * @brief Funkce pro generovani kodu
 *
 * @param node ukazatel na uzel stromu
 */
void generate(Node* node);
void generate_expr(Node* node, VarType expr_type);
char* data_type(type);
NodeType get_rhs(NodeType type);

#endif //GENERATOR_H
