#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "stringlib.h"
#include <stdio.h>
#include <stdbool.h>

typedef enum {
    TYPE_UNDEFINED, // nedefinovana promenna
    TYPE_INT,      // i32
    TYPE_FLOAT,    // f64
    TYPE_STRING,   // []u8
    TYPE_VOID,     // void
    TYPE_BOOL     // bool
} VarType;

typedef struct SymVal {
    bool is_const;
    bool is_func;
    bool is_defined;
    bool is_used;
    int type;
} SymbolValue;

typedef struct {
    String* key;        // identifikator
    VarType type;       // datovy typ
    void* value;        // hodnota promenne nebo funkci
    bool is_occupied;   // overi, zda uz obsazeno 
} SymbolTableEntry;

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
 * @brief Inserts a symbol into the symbol table.
 * 
 * Inserts a new symbol with its key, data type, and value into the symbol table.
 * If the table is full, it resizes the table to accommodate more entries.
 * 
 * @param table The symbol table where the symbol will be inserted.
 * @param key The key (identifier) of the symbol.
 * @param type The data type of the symbol.
 * @param value Pointer to the value associated with the symbol.
 */
void insert_symbol(SymbolTable* table, String* key, VarType type, void* value);

/**
 * @brief Finds a symbol in the symbol table.
 * 
 * Searches for a symbol by its key in the symbol table. If found, it returns a pointer to its value.
 * 
 * @param table The symbol table to search in.
 * @param key The key (identifier) of the symbol to search for.
 * @return void* Pointer to the symbol's value, or NULL if the symbol is not found.
 */
void* find_symbol(SymbolTable* table, String* key);

/**
 * @brief Deletes a symbol from the symbol table.
 * 
 * Removes a symbol from the symbol table by its key. Frees memory associated with the key and the value.
 * 
 * @param table The symbol table from which the symbol will be removed.
 * @param key The key (identifier) of the symbol to remove.
 */
void delete_symbol(SymbolTable* table, String* key);

/**
 * @brief Resizes the symbol table to a new size.
 * 
 * This function is used to expand the symbol table when it becomes too full. It reallocates the table with a larger size and reinserts all existing entries.
 * 
 * @param table The symbol table to resize.
 * @param new_size The new size for the table.
 */
void resize_table(SymbolTable* table, int new_size);

/**
 * @brief Frees the memory associated with a symbol table entry's value.
 * 
 * This function frees the memory of the value stored in a symbol table entry. It is used internally when deleting or resizing the table.
 * 
 * @param entry The entry whose value needs to be freed.
 */
void free_value(SymbolTableEntry* entry);

/**
 * @brief Frees the memory associated with a symbol table.
 * 
 * Frees all entries in the symbol table, including keys and values, and then frees the table itself.
 * 
 * @param table The symbol table to free.
 */
void free_symbol_table(SymbolTable* table);

#endif
