#ifndef _SYMSTACK_H
#define _SYMSTACK_H

#include "symtable.h"

// element tabulky symbolu
typedef struct symbol_stack_item {
    String* key;               // klic
    VarType type;              // typ
    
    union {                    // informace o symbolu
        FunctionInfo func_info; 
        VariableInfo var_info; 
    };

    struct symbol_stack_item* next;  // ukazatel na dalsi element
} SymbolStackItem;

// zasobnik symbolu
typedef struct {
    SymbolStackItem* top;  // ukazatel na vrchol
} SymbolStack;

SymbolStack* create_symbol_stack();
void push_function(SymbolStack* stack, String* key, FunctionInfo func_info);
void push_variable(SymbolStack* stack, String* key, VariableInfo var_info);
void pop_symbol(SymbolStack* stack);
SymbolStackItem* top_symbol(SymbolStack* stack);
void pop_until_symbol(SymbolStack* stack, String* key);
void free_symbol_stack(SymbolStack* stack);

#endif
