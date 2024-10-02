#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    char* data;      
    size_t length;   
    size_t capacity; 
} String;

String* create_empty_string();
String* create_string(const char* c_str);
void free_string(String* str);
String* copy_string(const String* source);
int compare_strings(const String* str1, const String* str2);
void append_string(String* dest, const String* src);
void resize_string(String* str, size_t new_capacity);
int find_substring(const String* str, const String* substr);

#endif
