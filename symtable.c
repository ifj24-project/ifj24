#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include "error/error.h"
#include "stringlib.h"
#include "symtable.h"

SymbolTable* create_symbol_table(int size) {
    SymbolTable* table = malloc(sizeof(SymbolTable));
     if (table == NULL) {
        ThrowError(99);
    }
    table->size = size;
    table->count = 0;
    table->table = calloc(size, sizeof(SymbolTableEntry));
    if (table->table == NULL) {
        free(table); // uvolnujeme pamet, pokud se allokace nepovedla
        ThrowError(99); 
    }
    return table;
}

unsigned int hash(String* key, int table_size) {
    unsigned long int hash_value = 0;
    for (int i = 0; i < key->length; i++) {
        hash_value = hash_value * 31 + key->data[i];  // hash_value * 31 == (hash_value << 5) - hash_value
    }
    return hash_value % table_size;
}

unsigned int second_hash(String* key, int table_size) {
    unsigned long int hash_value = 0;
    for (int i = 0; i < key->length; i++) {
        hash_value = hash_value * 33 + key->data[i]; // pouzivame jine proste cislo pro 2. hash
    }
    return 1 + (hash_value % (table_size - 1)); // krok musi byt nenulovy
}

int find_slot(SymbolTable* table, String* key) {
    int index = hash(key, table->size); // 1. hash funkce
    int step = second_hash(key, table->size); // 2. hash funkce
    int original_index = index;

    // dvojite hashovani
    while (table->table[index].is_occupied && compare_strings(table->table[index].key, key) != 0) {
        index = (index + step) % table->size; // posuvame se o "step" pozici v pripade kolize
        if (index == original_index) {
            ThrowError(99); // tabulka je plna
        }
    }
    return index;
}

void insert_symbol(SymbolTable* table, String* key, VarType type, void* value) {
     if (table->count >= table->size * 0.75) {  // pokud tabulka je vyplena vic nez 75%, rozsirujeme
        resize_table(table, table->size * 2);  // rozsireni tabulky 
    }
  
    int index = find_slot(table, key);
        if (!table->table[index].is_occupied) {
            table->table[index].key = copy_string(key);
            table->table[index].type = type;
            switch (type) {
            case TYPE_INT:
            case TYPE_FLOAT:
            case TYPE_BOOL:
                table->table[index].value = value;   // value se predava primo
                break;

            case TYPE_STRING:
                table->table[index].value = copy_string((String*)value); // kopirovani stringu
                break;

            default:
                table->table[index].value = NULL; // pro undefined typ 
                break;
        }
            table->table[index].is_occupied = true;
            table->count++;
        }
}

void* find_symbol(SymbolTable* table, String* key) {
    int index = find_slot(table, key);
    if (index != -1 && table->table[index].is_occupied) {
        return table->table[index].value; // vratime hodnotu prvku
    }
    //  ThrowError(3);
    return NULL; // nenasli jsme prvek 
}

void delete_symbol(SymbolTable* table, String* key) {
    int index = find_slot(table, key);
    if (index != -1 && table->table[index].is_occupied) {
        free_string(table->table[index].key);
        free_value(&(table->table[index]));   
        table->table[index].key = NULL; // nastaveni ukazatelu na NULL po uvolneni
        table->table[index].value = NULL; // nastaveni hodnoty na NULL 
        table->table[index].is_occupied = false;
        table->count--;
    }
}

// rozsireni tabulky
void resize_table(SymbolTable* table, int new_size) {
    SymbolTableEntry* old_table = table->table;  // ulozime starou verzi tabulky
    int old_size = table->size;

    // vytvorime novou tabulku o vetsi velikosti
    table->table = calloc(new_size, sizeof(SymbolTableEntry));
    table->size = new_size;
    table->count = 0;

    // dodavame prvky stare tabulky do nove
    for (int i = 0; i < old_size; i++) {
        if (old_table[i].is_occupied) {
            int new_index = find_slot(table, old_table[i].key);

            // kopirujeme klic
            table->table[new_index].key = copy_string(old_table[i].key);

            // kopirujeme hodnotu (zalezi na typu)
            switch (old_table[i].type) {
                case TYPE_STRING:
                    table->table[new_index].value = copy_string((String*)old_table[i].value);
                    break;
                case TYPE_INT:
                    table->table[new_index].value = malloc(sizeof(int));
                    *(int*)(table->table[new_index].value) = *(int*)(old_table[i].value);
                    break;
                case TYPE_FLOAT:
                    table->table[new_index].value = malloc(sizeof(float));
                    *(float*)(table->table[new_index].value) = *(float*)(old_table[i].value);
                    break;
                case TYPE_BOOL:
                    table->table[new_index].value = malloc(sizeof(bool));
                    *(bool*)(table->table[new_index].value) = *(bool*)(old_table[i].value);
                    break;
                default:
                    table->table[new_index].value = NULL;
                    break;
            }

            table->table[new_index].type = old_table[i].type;
            table->table[new_index].is_occupied = true;
            table->count++;
        }
    }

    // uvolnujeme starou tabulku
    free(old_table);
}

void free_value(SymbolTableEntry* entry) {
    switch (entry->type) {
        case TYPE_STRING:
            free_string((String*)(entry->value));  // uvolnujeme string
            break;
        // jine typy - int, bool, float nepotrebuji uvolneni
        default:
            break;
    }
}

void free_symbol_table(SymbolTable* table) {
    for (int i = 0; i < table->size; i++) {
        if (table->table[i].is_occupied) {
            free_string(table->table[i].key);      // uvolnujeme string
            free_value(&(table->table[i]));   // uvolnujeme value, pokud je dynamicky allokovano 
        }
    }
    free(table->table);         // uvolnujeme pole prvku
    free(table);                // uvolnujeme tabulku
}
