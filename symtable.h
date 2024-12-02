/** 
* @file symtable.h
* @author Ariana Tomen (xtomen00)
*
* IFJ24
*
* @brief Definice struktury a funkci pro implementaci tabulky symbolu
*/

#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "stringlib.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Typy podporovane v tabulce symbolu.
 * 
 * Obsahuje zakladni datove typy, vcetne typu s podporou null hodnot a funkce/variable specifikace.
 */
typedef enum {
    TYPE_INT,      // i32
    TYPE_FLOAT,    // f64
    TYPE_STRING,   // []u8
    TYPE_BOOL,     // bool
    TYPE_UNDEFINED, // nedefinovana promenna
    TYPE_INT_NULL, // ?i32
    TYPE_FLOAT_NULL, // ?f64
    TYPE_STRING_NULL, // ?[]u8
    TYPE_FUNCTION,  // funkce
    TYPE_VARIABLE,  // promenna
    TYPE_VOID, // void
    TYPE_NULL // null
} VarType;

/**
 * @brief Struktura parametru funkce.
 * 
 * Obsahuje informace o jednom parametru funkce, vcetne jeho nazvu, datoveho typu
 * a ukazatele na dalsi parametr (pro vazany seznam).
 */
typedef struct FunctionParam {
    String* name;   /**< Jmeno parametru. */
    VarType type;   /**< Datovy typ parametru. */
    struct FunctionParam *next;  /**< Ukazatel na dalsi parametr ve vazanem seznamu. */
} FunctionParam;

/**
 * @brief Struktura reprezentujici funkci.
 * 
 * Uchovava informace o navratovem typu funkce a parametrech (ve forme vazaneho seznamu).
 */
typedef struct {
    VarType return_type;        /**< Navratovy typ funkce. */
    FunctionParam* params;      /**< Ukazatel na prvni parametr funkce. */
    int param_count;            /**< Pocet parametru funkce. */
} FunctionInfo;

/**
 * @brief Struktura reprezentujici promennou.
 * 
 * Obsahuje informace o datovem typu promenne, jeji konstante/stav zmeny a pouziti.
 */
typedef struct {
    VarType data_type;   /**< Datovy typ promenne. */
    bool is_const;       /**< Znacka indikujici, zda je promenna konstantni. */ 
    bool is_used;        /**< Znacka indikujici, zda byla promenna pouzita. */
    bool changed;        /**< Znacka indikujici, zda byla promenna zmenena. */
} VariableInfo;

/**
 * @brief Polozka tabulky symbolu.
 * 
 * Reprezentuje jednu polozku tabulky symbolu, obsahuje klic, typ (promenna nebo funkce)
 * a informace o funkci nebo promenne.
 */
typedef struct {
    String* key;         /**< Identifikator (klic). */
    VarType type;        /**< Typ polozky (promenna nebo funkce). */
    union {              /**< Informace o funkci nebo promenne. */
        FunctionInfo func_info;
        VariableInfo var_info;
    };
    bool is_occupied;    /**< Znacka indikujici, zda je polozka obsazena. */
    bool is_deleted;     /**< Znacka indikujici, zda byla polozka smazana. */
} SymbolTableEntry;

/**
 * @brief Tabulka symbolu.
 * 
 * Struktura uchovavajici informace o tabulce symbolu, obsahujici pole polozek, jejich pocet a velikost tabulky.
 */
typedef struct {
    SymbolTableEntry* table; /**< Pole pro ukladani prvku tabulky. */
    int size;                /**< Velikost tabulky. */
    int count;               /**< Aktualni pocet prvku v tabulce. */
} SymbolTable;

/**
 * @brief Checks if a number is a prime.
 * 
 * This function verifies whether the provided number is a prime.
 * A number is prime if it is greater than 1 and divisible only by 1 and itself.
 * 
 * @param num The number to be checked.
 * @return bool Returns true if the number is prime, otherwise false.
 */
bool is_prime(int num);

/**
 * @brief Finds the next prime number greater than the given number.
 * 
 * This function calculates the next prime number greater than the given input.
 * If the number is less than 2, it starts searching from 2.
 * 
 * @param num The starting number to search from.
 * @return int Returns the next prime number.
 */
int get_next_prime(int num);

/**
 * @brief Creates a new symbol table.
 * 
 * Allocates memory for a new symbol table and initializes its internal structure.
 * 
 * @param size The initial size of the symbol table.
 * @return SymbolTable* Pointer to the newly created symbol table.
 * @throws 99 If memory allocation fails.
 */
SymbolTable* create_symbol_table(int size);

/**
 * @brief Computes the hash value for a given key.
 * 
 * Uses a hashing function to calculate the hash value of a string key.
 * 
 * @param key The key for which the hash value will be calculated.
 * @param table_size The size of the symbol table, used for modulo operation.
 * @return unsigned int The resulting hash value.
 */
unsigned int hash(String* key, int table_size);

/**
 * @brief Computes the second hash value for double hashing.
 * 
 * This function is used for double hashing, which helps resolve collisions in the symbol table.
 * 
 * @param key The key for which the second hash value will be calculated.
 * @param table_size The size of the symbol table, used for modulo operation.
 * @return unsigned int The resulting second hash value, must be non-zero.
 */
unsigned int second_hash(String* key, int table_size);

/**
 * @brief Finds the appropriate slot for a symbol in the table using double hashing.
 * 
 * This function locates an empty slot or the slot with the given key using double hashing 
 * to resolve collisions. If the table is full, it throws an error.
 * 
 * @param table The symbol table to search in.
 * @param key The key for which a slot is being found.
 * 
 * @return int The index of the found slot (empty or containing the symbol with the matching key).
 * 
 * @throws 99 If the symbol table is full and no free slot is found.
 */
int find_slot(SymbolTable* table, String* key);

/**
 * @brief Inserts a function into the symbol table without parameters.
 * 
 * This function inserts a function with a given key and return type into the symbol table.
 * 
 * @param table The symbol table where the function will be inserted.
 * @param key The key (identifier) of the function.
 * @param return_type The return type of the function.
 */
void insert_function(SymbolTable* table, String* key, VarType return_type);

/**
 * @brief Adds a parameter to an existing function in the symbol table.
 * 
 * This function adds a new parameter to an already existing function in the symbol table.
 * 
 * @param table The symbol table where the function is stored.
 * @param key The key (identifier) of the function to which the parameter will be added.
 * @param new_param The new parameter to be added to the function.
 */
void push_parameter(SymbolTable* table, String* key, FunctionParam new_param);

/**
 * @brief Inserts a variable into the symbol table.
 * 
 * Inserts a new variable with its identifier, data type, and constness information into the symbol table.
 * If the table is full, it resizes the table to accommodate more entries.
 * 
 * @param table The symbol table where the variable will be inserted.
 * @param key The identifier (key) of the variable.
 * @param data_type The data type of the variable.
 * @param is_const Indicates whether the variable is a constant.
 */
void insert_variable(SymbolTable* table, String* key, VarType data_type, bool is_const);

/**
 * @brief Gets the type of a symbol from the symbol table.
 * 
 * Searches for a symbol by its key in the symbol table. If found, returns its type.
 * 
 * @param table The symbol table to search in.
 * @param key The identifier (key) of the symbol to search for.
 * @return VarType The type of the symbol, or TYPE_UNDEFINED if the symbol is not found.
 */
VarType get_symbol_type(SymbolTable* table, String* key);

/**
 * @brief Marks a variable as used.
 * 
 * Sets a flag indicating that the variable has been used in the code.
 * 
 * @param table The symbol table in which to mark the variable.
 * @param key The identifier (key) of the variable to mark.
 */
void mark_variable_as_used(SymbolTable* table, String* key);

/**
 * @brief Marks a variable in the symbol table as modified.
 *
 * Checks if a variable with the specified key is found in the symbol table and 
 * verifies it is not a constant. If the variable is non-constant, it marks 
 * the variable as modified. Throws an error if the variable is a constant or 
 * if it is not found in the table.
 *
 * @param table Pointer to the symbol table where the variable is stored.
 * @param key Key (identifier) of the variable to mark as modified.
 * @throws Error 9 if the variable is a constant and cannot be modified.
 * @throws Error 99 if the variable does not exist or is not of type variable.
 */
void mark_variable_as_changed(SymbolTable* table, String* key);

/**
 * @brief Checks for unmodified, non-constant variables in the symbol table.
 *
 * Iterates through the symbol table to identify any non-constant variables 
 * that have not been modified. If such variables are found, throws an error. 
 * Returns the count of unmodified, non-constant variables if no error is thrown.
 *
 * @param table Pointer to the symbol table containing variables to check.
 * @return int Number of non-constant variables that remain unmodified. Returns 
 *         0 if all non-constant variables are correctly marked as modified.
 * @throws Error 9 if any non-constant variable is found unmodified.
 */
int check_unmodified_variables(SymbolTable* table);

/**
 * @brief Finds a symbol in the symbol table.
 * 
 * Searches for a symbol by its key in the symbol table. If found, returns a pointer to its associated information.
 * If the symbol is a function, it returns a pointer to its FunctionInfo.
 * If the symbol is a variable, it returns a pointer to its VariableInfo.
 * 
 * @param table The symbol table to search in.
 * @param key The key (identifier) of the symbol to search for.
 * @return void* A pointer to the symbol's FunctionInfo if it's a function, 
 *               or a pointer to the symbol's VariableInfo if it's a variable, 
 *               or NULL if the symbol is not found.
 */
void* find_symbol(SymbolTable* table, String* key);

/**
 * @brief Checks for unused variables in the symbol table.
 * 
 * This function iterates through the symbol table and checks for variables
 * that have not been used. If all variables have been used, it returns 0.
 * If there are any unused variables, it triggers ThrowError with error code 9.
 * 
 * @param table Pointer to the symbol table.
 * @return int Returns 0 if all variables have been used.
 * @throws 9 If there are unused variables in the symbol table.
 */
int check_unused_variables(SymbolTable* table);

/**
 * @brief Deletes a symbol from the symbol table.
 * 
 * Removes a symbol from the symbol table by its key. Frees memory associated with the key and parameters.
 * 
 * @param table The symbol table from which the symbol will be removed.
 * @param key The identifier (key) of the symbol to remove.
 */
void delete_symbol(SymbolTable* table, String* key);

/**
 * @brief Resizes the symbol table.
 * 
 * This function is used to expand the symbol table when it becomes too full.
 * 
 * @param table The symbol table to resize.
 * @param new_size The new size for the table.
 */
void resize_table(SymbolTable* table, int new_size);

/**
 * @brief Frees the memory associated with the symbol table.
 * 
 * Frees all entries in the symbol table, including keys and parameters, and then frees the table itself.
 * 
 * @param table The symbol table to free.
 */
void free_symbol_table(SymbolTable* table);

#endif
