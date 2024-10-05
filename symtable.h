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
} VarType;

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

SymbolTable* create_symbol_table(int size);
unsigned int hash(String* key, int table_size);
unsigned int second_hash(String* key, int table_size);
int find_slot(SymbolTable* table, String* key);
void insert_symbol(SymbolTable* table, String* key, VarType type, void* value);
void* find_symbol(SymbolTable* table, String* key);
void delete_symbol(SymbolTable* table, String* key);
void resize_table(SymbolTable* table, int new_size);
void free_value(SymbolTableEntry* entry);
void free_symbol_table(SymbolTable* table);

#endif
