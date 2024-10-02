#include <stringlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

String* create_empty_string() {
    String* str = (String*)malloc(sizeof(String));
    str->length = 0;
    str->capacity = 16; 
    str->data = (char*)malloc(str->capacity * sizeof(char));
    str->data[0] = '\0'; 
    return str;
}

String* create_string(const char *initial_data) {
    String *new_str = (String *)malloc(sizeof(String));
    if (new_str == NULL) {
        return NULL;
    }
    new_str->length = strlen(initial_data);
    new_str->capacity = new_str->length + 1;
    new_str->data = (char *)malloc(new_str->capacity);
    if (new_str->data == NULL) {
        free(new_str);
        return NULL;
    }
    strcpy(new_str->data, initial_data);
    return new_str;
}

void free_string(String* str) {
    if (str != NULL) {
        free(str->data);  
        free(str);       
    }
}

String* copy_string(const String *str) {
    if (!str) return NULL;
    return create_string(str->data);
}


int compare_strings(const String* str1, const String* str2) {
    if (!str1 || !str2) return -1;
    return strcmp(str1->data, str2->data);
}

String* concat_strings(const String *str1, const String *str2) {
    if (!str1 || !str2) return NULL;
    String *new_str = (String *)malloc(sizeof(String));
    if (new_str == NULL) return NULL;

    new_str->length = str1->length + str2->length;
    new_str->capacity = new_str->length + 1;

    new_str->data = (char *)malloc(new_str->capacity);
    if (new_str->data == NULL) {
        free(new_str);
        return NULL;
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
    if (!str || start >= end || end > str->length) return NULL;
    String *substr = (String *)malloc(sizeof(String));
    if (substr == NULL) return NULL;

    substr->length = end - start;
    substr->capacity = substr->length + 1;

    substr->data = (char *)malloc(substr->capacity);
    if (substr->data == NULL) {
        free(substr);
        return NULL;
    }
    strncpy(substr->data, str->data + start, substr->length);
    substr->data[substr->length] = '\0'; 
    return substr;
}

unsigned int hash_string(const String *str, int table_size) {
    unsigned long hash = 0;
    for (size_t i = 0; i < str->length; i++) {
        hash = hash * 31 + str->data[i]; 
    }
    return hash % table_size;  
}
