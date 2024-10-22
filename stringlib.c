#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include "error/error.h"
#include "stringlib.h"

String* create_empty_string() {
    String* str = (String*)malloc(sizeof(String));
    if (str == NULL) {
        ThrowError(99);
    }
    str->length = 0;
    str->capacity = 16; 
    str->data = (char*)malloc(str->capacity * sizeof(char));
    if (str->data == NULL) {
        free(str);
        ThrowError(99);
    }
    str->data[0] = '\0'; 
    return str;
}

String* create_string(const char *initial_data) {
    String *new_str = (String *)malloc(sizeof(String));
    if (new_str == NULL) {
        ThrowError(99);
    }
    new_str->length = strlen(initial_data);
    new_str->capacity = new_str->length + 1;
    new_str->data = (char *)malloc(sizeof(char) * new_str->capacity);
    if (new_str->data == NULL) {
        free(new_str);
        ThrowError(99);
    }
    strcpy(new_str->data, initial_data);
    return new_str;
}

void free_string(String* str) {
    if (str != NULL) {
        printf("string %s\n", str->data);
        free(str->data);  
        free(str);       
    }
}

String* copy_string(const String *str) {
    if (!str) ThrowError(4);
    return create_string(str->data);
}


int compare_strings(const String* str1, const String* str2) {
    if (!str1 || !str2) ThrowError(4);
    return strcmp(str1->data, str2->data);
}

String* concat_strings(const String *str1, const String *str2) {
    if (!str1 || !str2) ThrowError(4);
    String *new_str = (String *)malloc(sizeof(String));
    if (new_str == NULL) ThrowError(99);

    new_str->length = str1->length + str2->length;
    new_str->capacity = new_str->length + 1;

    new_str->data = (char *)malloc(sizeof(char) * new_str->capacity);
    if (new_str->data == NULL) {
        free(new_str);
        ThrowError(99);
    }
    strcpy(new_str->data, str1->data);
    strcat(new_str->data, str2->data);
    return new_str;
}

void resize_string(String* str, size_t new_capacity) {
    if (new_capacity > str->capacity) {
        str->capacity = new_capacity;
        str->data = (char*)realloc(str->data, str->capacity * sizeof(char));
    }
}

String* substring_string(const String *str, size_t start, size_t end) {
    if (!str || start >= end || end > str->length) ThrowError(4);
    String *substr = (String *)malloc(sizeof(String));
    if (substr == NULL) ThrowError(99);

    substr->length = end - start;
    substr->capacity = substr->length + 1;

    substr->data = (char *)malloc(sizeof(char) * substr->capacity);
    if (substr->data == NULL) {
        free(substr);
        ThrowError(99);
    }
    strncpy(substr->data, str->data + start, substr->length);
    substr->data[substr->length] = '\0'; 
    return substr;
}
