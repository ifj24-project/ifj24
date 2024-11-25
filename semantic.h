/** 
* @file semantic.h
* @author Patrik Mokrusa (xmokrup00)
*
* IFJ24
*
* @brief Interface semantickeho analyzatoru
*/

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"

/**
 * @brief pomocna fce pro uvolnovani pameti pri erroru
 * @param code error number
 */
void semantic_wrapper_ThrowError(int code);

/**
 * @brief rekurzivni funkce pro kontrolu skoro veskere semantiky
 * @param parse_tree ukazatel na derivacni strom
 * @param global_sym_table ukazatel na globalni tabulku symbolu
 * @param global_func_key (interni pass parameter) NULL pri externim volani
 * @param local_sum_table (interni pass parameter) NULL pri externim volani
 * 
 * @throw hodi error pri nalezu chyby viz zadani
 */
void semantic_scan(Node* parse_tree, SymbolTable* global_sym_table, String* global_func_key, SymbolTable* local_sym_table);

/**
 * @brief zjisti zda v podstomu existuje uzel typu return_N
 * 
 * @returns true pokud uzel najde, jinak ne
 */
bool find_ret_statement(Node* node);

/**
 * @brief zjisti, zda ma desetinne cislo nulovou desetinnou cast
 */
bool is_whole_float(double value);

/**
 * @brief zkontroluje semantiku ve vyrazech
 * @return vraci celkovy sjednocenny typ vyrazu
 */
VarType semantic_expr(Node* node, SymbolTable* global_table, SymbolTable* local_table);

/**
 * @brief konvertne, pokud mozno, vyraz na int
 * @return -1 pokud konvertovat nelze
 * @return 0 pokud se vyraz povedl zkonvertovat
 */
int expr_to_int(Node* node, SymbolTable* global_table, SymbolTable* local_table);

/**
 * @brief konvertne, pokud mozno, vyraz na float
 * @return -1 pokud konvertovat nelze
 * @return 0 pokud se vyraz povedl zkonvertovat
 */
int expr_to_flt(Node * node, SymbolTable* global_table, SymbolTable* local_table);


bool type_cmp(VarType x, VarType y);

#endif
