#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include "../error/error.h"
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

void insert_function(SymbolTable* table, String* key, VarType return_type, FunctionParam* params, int param_count) {
    if (table->count >= table->size * 0.75) {  // pokud tabulka je vyplena vic nez 75%, rozsirujeme
    resize_table(table, table->size * 2);  // rozsireni tabulky 
    }
    int index = find_slot(table, key);
    if (!table->table[index].is_occupied) {
        table->table[index].key = copy_string(key);
        table->table[index].type = TYPE_FUNCTION;
        table->table[index].func_info.return_type = return_type;
        table->table[index].func_info.params = malloc(sizeof(FunctionParam) * param_count);
        for (int i = 0; i < param_count; i++) {
        table->table[index].func_info.params[i] = params[i];
        }
        table->table[index].func_info.param_count = param_count;
        table->table[index].is_occupied = true;
        table->count++;
    }
}

void insert_variable(SymbolTable* table, String* key, VarType data_type, bool is_const) {
    if (table->count >= table->size * 0.75) {  // pokud tabulka je vyplena vic nez 75%, rozsirujeme
    resize_table(table, table->size * 2);  // rozsireni tabulky 
    }
    int index = find_slot(table, key);
    if (!table->table[index].is_occupied) {
        table->table[index].key = copy_string(key);
        table->table[index].type = TYPE_VARIABLE;
        table->table[index].var_info.data_type = data_type;
        table->table[index].var_info.is_const = is_const;
        table->table[index].var_info.is_used = false;
        table->table[index].is_occupied = true;
        table->count++;
    }
}

VarType get_symbol_type(SymbolTable* table, String* key) {
    int index = find_slot(table, key);
    if (index != -1 && table->table[index].is_occupied) {
        return table->table[index].type;
    }
    return TYPE_UNDEFINED;  // nenasli jsme symbol
}

void mark_variable_as_used(SymbolTable* table, String* key) {
    int index = find_slot(table, key);
    if (index != -1 && table->table[index].is_occupied && table->table[index].type == TYPE_VARIABLE) {
        table->table[index].var_info.is_used = true;  // oznacime promennou jako pouzitou 
    }
}

void delete_symbol(SymbolTable* table, String* key) {
    int index = find_slot(table, key);
    if (index != -1 && table->table[index].is_occupied) {
        free_string(table->table[index].key);
        if (table->table[index].type == TYPE_FUNCTION) {
            free(table->table[index].func_info.params);  // uvolnujeme parametry funkce
        }
        table->table[index].is_occupied = false;
        table->count--;
    }
}

void resize_table(SymbolTable* table, int new_size) {
    // ukladame starou tabulku
    SymbolTableEntry* old_table = table->table;  
    int old_size = table->size;

    // vytvarime novou tabulku s vetsi velikosti
    table->table = calloc(new_size, sizeof(SymbolTableEntry));
    if (table->table == NULL) {
        free(old_table); // uvolnujeme starou tabulku v pripade chyby
        ThrowError(99);  // chyba allokace 
    }

    table->size = new_size; // aktualizujeme velikost tabulky
    table->count = 0;       // resetujeme altualni pocet elementu

    // prenosime elementy ze stare tabulky do nove
    for (int i = 0; i < old_size; i++) {
        if (old_table[i].is_occupied) {
            // hledame slot
            int new_index = find_slot(table, old_table[i].key);
            
            // kopirujeme klic
            table->table[new_index].key = copy_string(old_table[i].key);
            table->table[new_index].type = old_table[i].type;

            // pokud je to funkce, kopirujeme parametry a navratovy typ
            if (old_table[i].type == TYPE_FUNCTION) {
                table->table[new_index].func_info.return_type = old_table[i].func_info.return_type;
                table->table[new_index].func_info.param_count = old_table[i].func_info.param_count;
                
                // Allokujeme pamet pro parametry a kopirujeme je 
                table->table[new_index].func_info.params = malloc(sizeof(FunctionParam) * old_table[i].func_info.param_count);
                for (int j = 0; j < old_table[i].func_info.param_count; j++) {
                    table->table[new_index].func_info.params[j] = old_table[i].func_info.params[j];
                }
            } else if (old_table[i].type == TYPE_VARIABLE) {
                // pokud je to promenna, kopirujeme jeji informace
                table->table[new_index].var_info.data_type = old_table[i].var_info.data_type;
                table->table[new_index].var_info.is_const = old_table[i].var_info.is_const;
                table->table[new_index].var_info.is_used = old_table[i].var_info.is_used;
            }

            // oznacime novy element jako obsazeny
            table->table[new_index].is_occupied = true;
            table->count++;
        }
    }

    // uvolnujeme starou tabulku
    free(old_table);
}

void free_symbol_table(SymbolTable* table) {
    for (int i = 0; i < table->size; i++) {
        if (table->table[i].is_occupied) {
            free_string(table->table[i].key);  // uvolnujeme klic
            if (table->table[i].type == TYPE_FUNCTION) {
                free(table->table[i].func_info.params);  // uvolnujeme parametry funkce
            }
        }
    }
    free(table->table);  // uvolnujeme tabulku
    free(table);
}
