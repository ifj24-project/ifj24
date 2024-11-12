#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include "error/error.h"
#include "stringlib.h"
#include "symtable.h"

SymbolTable* create_symbol_table(int size) {
    size = get_next_prime(size); // velikost tabulky musi byt prvocislo pokud chceme pouzivat dvojity hash

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

bool is_prime(int num){
    if (num < 2) return false;

    for (int i = 2; i <= (num/2); i++)
    {
        if (num % i == 0) return false;
    }

    return true;
}

int get_next_prime(int num){
    if (num < 2) num = 2;
    num++;
    while (is_prime(num) == false)
    {
        num++;
    }
    
    return num;
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
        hash_value = hash_value * 37 + key->data[i]; // pouzivame jine proste cislo pro 2. hash
    }
    return 1 + (hash_value % (table_size - 1)); // krok musi byt nenulovy
}

int find_slot(SymbolTable* table, String* key) {
    int index = hash(key, table->size); // 1. hash funkce
    int step = second_hash(key, table->size); // 2. hash funkce
    int original_index = index;
    int checked_slots = 0;  // pocitadlo prochazenych slotu
    int deleted_index = -1;
    
    while (table->table[index].is_occupied || table->table[index].is_deleted) {
        if (table->table[index].is_occupied && compare_strings(table->table[index].key, key) == 0) {
            return index;
        }
         if (deleted_index == -1 && table->table[index].is_deleted) {
            deleted_index = index;  
        }
        
        index = (index + step) % table->size;
        checked_slots++;
        
        if (index == original_index && checked_slots >= table->size) {
            ThrowError(99); // tabulka je plna
        }
    }
    return (deleted_index != -1) ? deleted_index : index;
}

void insert_function(SymbolTable* table, String* key, VarType return_type) {
    if (table->count >= table->size * 0.75) {   // pokud tabulka je vyplena vic nez 75%, rozsirujeme
        resize_table(table, table->size * 2);  // rozsireni tabulky
    }
    
    int index = find_slot(table, key);
    if (!table->table[index].is_occupied || table->table[index].is_deleted) {
        table->table[index].key = copy_string(key);
        table->table[index].type = TYPE_FUNCTION;
        table->table[index].func_info.return_type = return_type;
        table->table[index].func_info.params = NULL;  // zpocatku nejsou zadne parametry
        table->table[index].func_info.param_count = 0;  // nastaveni poctu parametru na 0
        table->table[index].is_occupied = true;
        table->table[index].is_deleted = false;
        table->count++;
    } else {
        // pokud funkce jiz existuje
        ThrowError(99);  
    }
}

void push_parameter(SymbolTable* table, String* key, FunctionParam new_param) {
    int index = find_slot(table, key);
    
    if (index != -1 && table->table[index].is_occupied && table->table[index].type == TYPE_FUNCTION) {
        // pridavame parametr funkci
        FunctionParam* current_param = table->table[index].func_info.params;
        FunctionParam* new_param_ptr = malloc(sizeof(FunctionParam));
        if (new_param_ptr == NULL) {
            ThrowError(99);  // chyba allokace
        }

        new_param_ptr->name = copy_string(new_param.name);  // kopirujeme jmeno parametru
        new_param_ptr->type = new_param.type;  // nastavime typ parametru
        new_param_ptr->next = NULL;  //  inicializace ukazatelu na dalsi parametr

        if (current_param == NULL) {
            // pokud je to prvni parametr
            table->table[index].func_info.params = new_param_ptr;
        } else {
            // hledame posledni parametr
            while (current_param->next != NULL) {
                current_param = current_param->next;
            }
            current_param->next = new_param_ptr;  // pridavame novy parametr na konec seznamu
        }

        // zvetsime pocet parametru
        table->table[index].func_info.param_count++;
    } else {
        // pokud jsme nenasli funkce
        ThrowError(99);  
    }
}

void insert_variable(SymbolTable* table, String* key, VarType data_type, bool is_const) {
    if (table->count >= table->size * 0.75) {  // pokud tabulka je vyplena vic nez 75%, rozsirujeme
    resize_table(table, table->size * 2);  // rozsireni tabulky 
    }
    int index = find_slot(table, key);
    if (!table->table[index].is_occupied || table->table[index].is_deleted) {
        table->table[index].key = copy_string(key);
        table->table[index].type = TYPE_VARIABLE;
        table->table[index].var_info.data_type = data_type;
        table->table[index].var_info.is_const = is_const;
        table->table[index].var_info.is_used = false;
        table->table[index].is_occupied = true;
        table->table[index].is_deleted = false;
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

void mark_variable_as_changed(SymbolTable* table, String* key) {
    int index = find_slot(table, key);
    if (index != -1 && table->table[index].is_occupied && table->table[index].type == TYPE_VARIABLE) {
        VariableInfo* var_info = &table->table[index].var_info;
        // kontrola: pokud promenna je konstantni 
        if (var_info->is_const) {
            ThrowError(9);  // chyba: nemuzeme menit konstantni promenne
        }
        var_info->changed = true;  // nastavime na true modifikovatelnou promennu 
       } else {
        ThrowError(99);  // chyba: promenna nenajdena nebo neni to promenna
    }
}

int check_unmodified_variables(SymbolTable* table) {
    int count = 0;

    for (int i = 0; i < table->size; i++) {
        if (table->table[i].is_occupied && table->table[i].type == TYPE_VARIABLE) {
            VariableInfo* var_info = &table->table[i].var_info;
            if (!var_info->is_const && !var_info->changed) {
                fprintf(stderr, "var unchanged: %s\n", table->table[i].key->data);
                count++;
            }
        }
    }

    return count;  // vrati 0, pokud vsechny promenne jsou korektne oznaceny jako modifikovatelne 
}

void* find_symbol(SymbolTable* table, String* key) {
    int index = find_slot(table, key);
    if (index != -1 && table->table[index].is_occupied) {
        // pokud element je funkce
        if (table->table[index].type == TYPE_FUNCTION) {
            return &table->table[index].func_info;  // vratime ukazatel na informace o funkci
        }
        // pokud element je promenna
        else if (table->table[index].type == TYPE_VARIABLE) {
            return &table->table[index].var_info;    // vracime ukazatel na informace o promenne
        }
    }
    return NULL;  // nenasli jsme symbol 
}

int check_unused_variables(SymbolTable* table) {
    int unused_count = 0;
    for (int i = 0; i < table->size; i++) {
        if (table->table[i].is_occupied && table->table[i].type == TYPE_VARIABLE) {
            // kontrola pouziti promenne 
            if (!table->table[i].var_info.is_used) {
                fprintf(stderr, "var unused: %s\n", table->table[i].key->data);
                unused_count++;  // zvetsime pocitadlo nepouzitych promennych
            }
        }
    }

    return unused_count;
}

void delete_symbol(SymbolTable* table, String* key) {
    int index = find_slot(table, key);
    if (index != -1 && table->table[index].is_occupied && !table->table[index].is_deleted) {
        free_string(table->table[index].key);
        if (table->table[index].type == TYPE_FUNCTION) {
           FunctionParam* current_param = table->table[index].func_info.params;
            while (current_param != NULL) {
                FunctionParam* temp = current_param;       // docasny ukazatel na aktualni parametr 
                current_param = current_param->next;       // prechod do dalsiho parametru
                free_string(temp->name);                  // uvolnujeme jmeno parametru
                free(temp);                // uvolnujeme parametr
            }
        }
        table->table[index].is_deleted = true;
        table->table[index].is_occupied = false;
    }
}

void resize_table(SymbolTable* table, int new_size) {
    new_size = get_next_prime(new_size); // velikost tabulky musi byt prvocislo pokud chceme pouzivat dvojity hash

    // ukladame starou tabulku
    SymbolTableEntry* old_table = table->table;  
    int old_size = table->size;

    // vytvarime novou tabulku s vetsi velikosti
    table->table = calloc(new_size, sizeof(SymbolTableEntry));
    if (table->table == NULL) {
        //??
        SymbolTable* temp_table = malloc(sizeof(SymbolTable));
        if (temp_table == NULL) ThrowError(99);
        temp_table->count = table->count;
        temp_table->size = old_size;
        temp_table->table = old_table;
        free_symbol_table(temp_table);  // uvolnujeme starou tabulku v pripade chyby
        ThrowError(99);  // chyba allokace 
    }

    table->size = new_size; // aktualizujeme velikost tabulky
    table->count = 0;       // resetujeme altualni pocet elementu

    // prenosime elementy ze stare tabulky do nove
    for (int i = 0; i < old_size; i++) {
        if (old_table[i].is_occupied) {
            // pokud je to funkce, kopirujeme parametry a navratovy typ
            if (old_table[i].type == TYPE_FUNCTION) {
                insert_function(table, old_table[i].key, old_table[i].func_info.return_type);
                table->table[find_slot(table, old_table[i].key)].func_info.params = old_table[i].func_info.params;
                old_table[i].func_info.params = NULL; // nastavim ve stare tabulce na NULL aby free_symbol_table je nesmazal
                table->table[find_slot(table, old_table[i].key)].func_info.param_count = old_table[i].func_info.param_count;
                
            } else if (old_table[i].type == TYPE_VARIABLE) {
                // pokud je to promenna, kopirujeme jeji informace
                 insert_variable(table, old_table[i].key, old_table[i].var_info.data_type, old_table[i].var_info.is_const);
                 int index = find_slot(table, old_table[i].key);
                 table->table[index].var_info.is_used = old_table[i].var_info.is_used;
                 table->table[index].var_info.changed = old_table[i].var_info.changed;
            }
        }
    }
    // uvolnujeme starou tabulku
    SymbolTable* temp_table = malloc(sizeof(SymbolTable));
    if (temp_table == NULL) ThrowError(99);
    temp_table->count = table->count;
    temp_table->size = old_size;
    temp_table->table = old_table;
    free_symbol_table(temp_table);
}

void free_symbol_table(SymbolTable* table) {
    for (int i = 0; i < table->size; i++) {
        if (table->table[i].is_occupied) {
            free_string(table->table[i].key);  // uvolnujeme klic
            if (table->table[i].type == TYPE_FUNCTION) {
                FunctionParam* current_param = table->table[i].func_info.params;
                while (current_param != NULL) {
                    FunctionParam* temp = current_param; // docasny ukazatel
                    current_param = current_param->next;  // prechod do dalsiho parametru
                    free_string(temp->name); // uvolnujeme jmeno parametru
                    free(temp); // uvolnujeme parametr
                }
            }
        }
    }
    free(table->table);  // uvolnujeme tabulku
    free(table);
}
