#include <stdlib.h>
#include "symstack.h"
#include "../error/error.h"
#include "stringlib.h"
#include <malloc.h>

// Vytvoreni zasobniku symbolu
SymbolStack* create_symbol_stack() {
    SymbolStack* stack = malloc(sizeof(SymbolStack));
    if (stack == NULL) {
        ThrowError(99);  // Chyba alokace pameti
    }
    stack->top = NULL;  // Inicializace prazdneho zasobniku
    return stack;
}

// Pridani funkce do zasobniku
void push_function(SymbolStack* stack, String* key, FunctionInfo func_info) {
    SymbolStackItem* new_item = malloc(sizeof(SymbolStackItem));
    if (new_item == NULL) {
        ThrowError(99);  // Chyba alokace pameti
    }
    
    new_item->key = copy_string(key);       // Kopirujeme klic
    new_item->type = TYPE_FUNCTION;         // Nastavujeme typ jako funkci
    new_item->func_info = func_info;        // Kopirujeme informace o funkci
    new_item->next = stack->top;            // Pridavame novy prvek na vrchol zasobniku
    stack->top = new_item;
}

// Pridani promenne do zasobniku
void push_variable(SymbolStack* stack, String* key, VariableInfo var_info) {
    SymbolStackItem* new_item = malloc(sizeof(SymbolStackItem));
    if (new_item == NULL) {
        ThrowError(99);  // Chyba alokace pameti
    }

    new_item->key = copy_string(key);       // Kopirujeme klic
    new_item->type = TYPE_VARIABLE;         // Nastavujeme typ jako promennou
    new_item->var_info = var_info;          // Kopirujeme informace o promenne
    new_item->next = stack->top;            // Pridavame novy prvek na vrchol zasobniku
    stack->top = new_item;
}

// Odebrani symbolu z vrcholu zasobniku
void pop_symbol(SymbolStack* stack) {
    if (stack->top == NULL) {
        ThrowError(99);  // Chyba: zasobnik je prazdny
    }

    SymbolStackItem* top_item = stack->top;  // Ziskavame vrchol zasobniku
    stack->top = top_item->next;             // Aktualizujeme vrchol zasobniku

    free_string(top_item->key);              // Uvolnujeme klic
    if (top_item->type == TYPE_FUNCTION) {
        FunctionParam* param = top_item->func_info.params;
        while (param != NULL) {
            FunctionParam* temp = param;
            param = param->next;
            free_string(temp->name);  // Uvolnujeme jmeno parametru
            free(temp);               // Uvolnujeme parametr
        }
    }
    free(top_item);                    // Uvolnujeme prvek
}

// Ziskani symbolu z vrcholu zasobniku (bez odebrani)
SymbolStackItem* top_symbol(SymbolStack* stack) {
    if (stack->top == NULL) {
        ThrowError(99);  // Chyba: zasobnik je prazdny
    }
    return stack->top;  // Vraci prvek na vrcholu zasobniku
}

// Odebrani prvku ze zasobniku az do zadaneho klice
void pop_until_symbol(SymbolStack* stack, String* key) {
    while (stack->top != NULL) {
        if (compare_strings(stack->top->key, key) == 0) {
            break;  // Nasli jsme potrebny symbol, ukoncime odebirani
        }
        pop_symbol(stack);  // Odebirame prvky ze zasobniku, dokud nenajdeme symbol s klicem
    }
}

// Uvolneni pameti pro cely zasobnik
void free_symbol_stack(SymbolStack* stack) {
    while (stack->top != NULL) {
        SymbolStackItem* temp = stack->top;
        stack->top = stack->top->next;

        free_string(temp->key);  // Uvolnujeme klic
        if (temp->type == TYPE_FUNCTION) {
            // Uvolnujeme parametry funkce
            FunctionParam* param = temp->func_info.params;
            while (param != NULL) {
                FunctionParam* next_param = param->next;
                free_string(param->name);
                free(param);
                param = next_param;
            }
        }
        free(temp);  // Uvolnujeme prvek zasobniku
    }
    free(stack);  // Uvolnujeme samotny zasobnik
}
