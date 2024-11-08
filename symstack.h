#ifndef _SYMSTACK_H
#define _SYMSTACK_H

#include "symtable.h"

// Element tabulky symbolu
typedef struct symbol_stack_item {
    String* key;               // Klic
    VarType type;              // Typ
    
    union {                    // Informace o symbolu
        FunctionInfo func_info; 
        VariableInfo var_info; 
    };

    struct symbol_stack_item* next;  // Ukazatel na dalsi element
} SymbolStackItem;

// Zasobnik symbolu
typedef struct {
    SymbolStackItem* top;  // Ukazatel na vrchol
} SymbolStack;

SymbolStack* create_symbol_stack();
void push_function(SymbolStack* stack, String* key, FunctionInfo func_info);
void push_variable(SymbolStack* stack, String* key, VariableInfo var_info);
void pop_symbol(SymbolStack* stack);
SymbolStackItem* top_symbol(SymbolStack* stack);
void pop_until_symbol(SymbolStack* stack, String* key);
void free_symbol_stack(SymbolStack* stack);

#endif
