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

void generate(Node* node);
void generate_expr(Node* node, VarType expr_type);
char* data_type(char* type);

#endif //GENERATOR_H
