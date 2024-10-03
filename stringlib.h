#ifndef STRINGLIB_H
#define STRINGLIB_H

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char* data;      
    size_t length;   
    size_t capacity; 
} String;

String* create_empty_string();
String* create_string(const char *initial_data);
void free_string(String* str);
String* copy_string(const String *str);
int compare_strings(const String* str1, const String* str2);
String* concat_strings(const String *str1, const String *str2);
void resize_string(String* str, size_t new_capacity);
String* substring_string(const String *str, size_t start, size_t end);
unsigned int hash(String* key, int table_size);

#endif
