#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "stringlib.h"
#include <stdio.h>
#include <stdbool.h>

typedef enum {
    TYPE_INT,      // i32
    TYPE_FLOAT,    // f64
    TYPE_STRING,   // []u8
    TYPE_BOOL,     // bool
    TYPE_UNDEFINED, // nedefinovana promenna
    TYPE_FUNCTION,  // funkce
    TYPE_VARIABLE,  // promenna
    TYPE_VOID
} VarType;

// Struktura parametru funkce
typedef struct FunctionParam {
    String* name;   // jmeno parametru
    VarType type;   // typ
    struct FunctionParam *next;  // ukazatel na nasledujici parametr
} FunctionParam;

// Struktura funkce
typedef struct {
    VarType return_type;       // navratovy typ
    FunctionParam* params;     // parametry funkce
    int param_count;           // pocet parametru
} FunctionInfo;

// Struktura promenne
typedef struct {
    VarType data_type;   // datovy typ promenne
    bool is_const;       // je-li promenna konstantni 
    bool is_used;        // je-li promenna pouzita
} VariableInfo;

// Struktura pro zapis do tabulky symbolu
typedef struct {
    String* key;         // identifikator (klic)
    VarType type;        // typ (promenna nebo funkce)
    union {              // informace o funkci nebo promenne
        FunctionInfo func_info;
        VariableInfo var_info;
    };
    bool is_occupied;    // overi, zda uz obsazeno 
} SymbolTableEntry;

// Struktura tabulky
typedef struct {
    SymbolTableEntry* table; // pole pro ukladani prvku tabulky
    int size;                // velikost tabulky
    int count;               // aktualni pocet prvku 
} SymbolTable;


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
 * @brief Finds the appropriate slot for a symbol in the table.
 * 
 * This function uses double hashing to find an empty slot or a slot with the given key in the symbol table.
 * 
 * @param table The symbol table to search in.
 * @param key The key for which a slot is being found.
 * @return int The index of the found slot, or an error if the table is full.
 * @throws 99 If the table is full and no slot is found.
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
