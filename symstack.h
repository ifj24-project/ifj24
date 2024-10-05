#ifndef _SYMSTACK_H
#define _SYMSTACK_H

#include <stdio.h>
#include "symtable.h"

typedef struct symbol_stack_item {
    SymbolTableEntry entry;             // prvek tabulky symbolu (klic, hodnota atd)
    struct symbol_stack_item* next;     // ukazatel na nasledujici prvek 
} SymbolStackItem;

typedef struct {
    SymbolStackItem* top;               // ukazatel na vrchol zasobniku
} SymbolStack;

SymbolStack* create_symbol_stack();
void push_symbol(SymbolStack* stack, SymbolTableEntry entry);
SymbolTableEntry pop_symbol(SymbolStack* stack);
SymbolTableEntry top_symbol(SymbolStack* stack);
void pop_until_symbol(SymbolStack* stack, String* key);
void free_symbol_stack(SymbolStack* stack);

#endif
