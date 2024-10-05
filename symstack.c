#include <stdlib.h>
#include "symstack.h"
#include "error/error.h"
#include "stringlib.h"
#include <malloc.h>

SymbolStack* create_symbol_stack() {
    SymbolStack* stack = malloc(sizeof(SymbolStack));
    if (stack == NULL) {
        ThrowError(99);  // chyba allokace pameti
    }
    stack->top = NULL;
    return stack;
}

void push_symbol(SymbolStack* stack, SymbolTableEntry entry) {
    SymbolStackItem* new_item = malloc(sizeof(SymbolStackItem));
    if (new_item == NULL) {
        ThrowError(99);  // chyba allokace pameti
    }
    new_item->entry = entry;     // kopirujeme prvek tabulky symbolu do noveho prvku zasobniku
    new_item->next = stack->top; 
    stack->top = new_item;       // novy prvek == vrchol zasobniku
}

SymbolTableEntry pop_symbol(SymbolStack* stack) {
    if (stack->top == NULL) {
        ThrowError(99);  // chyba: zasobnik je prazdny
    }
    SymbolStackItem* top_item = stack->top;        // ted je vrcholem
    SymbolTableEntry entry = top_item->entry;      // entry == prvek na vrcholu
    stack->top = top_item->next;                   // vrchol je nasledujici prvek
    free(top_item);                                // uvolnujeme pamet
    return entry;
}

SymbolTableEntry top_symbol(SymbolStack* stack) {
    if (stack->top == NULL) {
        ThrowError(99);  // chyba: zasobnik je prazdny
    }
    return stack->top->entry;
}

void pop_until_symbol(SymbolStack* stack, String* key) {
    while (stack->top != NULL) {
        if (compare_strings(stack->top->entry.key, key) == 0) {
            break;  // nasli jsme symbol, zastavime pop
        }
        pop_symbol(stack);  
    }
}

void free_symbol_stack(SymbolStack* stack) {
    while (stack->top != NULL) {
        SymbolStackItem* temp = stack->top;
        stack->top = stack->top->next;
        free(temp);  // uvolnujeme kazdy prvek zasobniku
    }
    free(stack);  // uvolnujeme zasobnik
}
