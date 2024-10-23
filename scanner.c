#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "error.c"

/* TODO :
    - Functions which will clean up the memory from tokens and their values
    - Need lot of testing too Multiline and single line strings
    - How long Identifiers can be 
    - Nullables
    - Param,import as KW 
    - unicode value is right but dont print special characters (like [F8 = 248 = ø]),but prints up to 127 )
    - 123.123e123.12 (is lexical or syntax error) (2 dots in float)
    - could add function which will alocate memory so we dont have to do it in every function
    
    - comment ending in "\" sometimes messing up the code so dont type it
!!! DONT CHANGE THE ORDER OF ENUMS !!! (if I do make changes in Create_ID_or_Keyword,change in detecting keywords)
*/

int IsWhiteChar(char input){
    if (input == ' ' || input == '\t' || input == '\n' || (input >= 11 && input <= 13))
        return 1;
    else
        return 0;
}

char GetNotWhiteChar() {
    char temp = getchar();
    if (IsWhiteChar(temp)) {
        return GetNotWhiteChar();
    } 
    else 
        return temp;
}

void create_error(Token* token, int code){
    token->Category = TC_ERR;
    token->type = T_ERORR;
    token->value.code=code;
}

Token* Token_create(enum token_type type, enum token_Category category) {
    Token* token = malloc(sizeof(Token)); 
    if (token == NULL) {
        create_error(token,99);
    }
    else{
        token->type = type;
        token->Category = category;
    }
    return token;
}

void SkipComment() {
    while (1) {
        char next = getchar();
        if (next == '\n' || next == EOF) {
            break;
        }
    }
}

Token* Create_ID_or_Keyword(char first){
    Token* token = Token_create(T_ID, TC_ID);
    int length=60;
    token->value.ID_name = malloc(sizeof(char) * length);

    if (token->value.ID_name == NULL) {                 // malloc failed
        free(token->value.ID_name);
        create_error(token,99);
        return token;
    }

    int index = 0;
    token->value.ID_name[index] = first; // First character of the identifier
    index++;


    char Char;
    Char = getchar();
    while(1) {
        if (!isalnum(Char) && Char != '_'){     // end of ID
            ungetc(Char, stdin);
            break;
        }
        else {                                  // char in automata
            token->value.ID_name[index] = Char;
            index++;
            Char = getchar();
        }
    }
    
    token->value.ID_name[index] = '\0';
    //printf("Id: %s\n", token->value.ID_name);
    char *keywords[] = {"import","while","void", "var","u8","return","pub","null","f64","i32","if","fn","else","const"};
    for(int i = 0; i < 14; i++) {
        //printf("%s==%s \n", keywords[i], token->value.ID_name);
        if(strcmp(keywords[i], token->value.ID_name) == 0) {
            //printf("we found KW\n");
            if (token->value.ID_name == "null") {
                token->type = T_ID;
                return token;
            }
            
            // 39 is the first keyword, so I flipped list of keywords and minused the position of the index
            token->type = 39 - i;
            return token;
        }
    }
    return token;
    
}

Token* numbers(char first){
    Token* token = Token_create(T_Integer, TC_VALUE);

    char numbers[256]; 
    int index = 0;
    numbers[index] = first;
    index++;

    char Char;
    Char = getchar();

    while (isdigit(Char)) {         
        numbers[index] = Char;
        index++;
        Char = getchar();
    }
    // Integer part Done    
    
    if (Char == '.'){
        numbers[index] = Char;
        index++;
        Char = getchar();
        if(!isdigit(Char) && Char!='e' && Char!='E'){
            ungetc(Char, stdin);
            token = Token_create(T_ERORR, TC_ERR);
            token->value.code=1;
            return token;
        } 
        while (isdigit(Char)) {
            numbers[index] = Char;
            index++;
            Char = getchar();
        }
    }

    // decimal part done
    //numbers[index] = '\0';
    //printf("Number: %s\n", numbers);
    if (Char == 'e' || Char == 'E') {   // Exponentn part
        numbers[index] = Char;
        index++;
        Char = getchar();
        
        if (Char == '+' || Char == '-') {
            numbers[index] = Char;
            index++;
            Char = getchar();
            if (!isdigit(Char)){
                ungetc(Char, stdin);
                token = Token_create(T_ERORR, TC_ERR);
                token->value.code=1;
                return token;
            }
        }

        if(isdigit(Char)){
            while (isdigit(Char)) {
                numbers[index] = Char;
                index++;
                Char = getchar();
            }
        }

        else{
            ungetc(Char, stdin);
            token = Token_create(T_ERORR, TC_ERR);
            token->value.code=1;
            return token;
        }
    }
    ungetc(Char, stdin);        // we are done with numbers push back the last read character
    numbers[index] = '\0';
    // Done with Exponents
    //printf("Number: %s\n", numbers);
    if (strchr(numbers, '.') != NULL || strchr(numbers, 'e') != NULL || strchr(numbers, 'E') != NULL) {
        token->type = T_Float;
        token->value.Float = atof(numbers);    // Convert string to double
    } 
    else {
        token->type=T_Integer;
        token->value.integer = atoi(numbers);     // Convert string to int
    }
    return token;
}

int escape_sequence() {
    char curr = getchar(); 
    switch (curr) {
        case '\"':                          // this "  
            return '\"';
        case 'n':                           // new line
            return '\n';
        case 'r':                           // reset to the line start
            return '\r';
        case 't' :                          // tab
            return '\t';
        case '\\':                       
            return '\\';
        case 'x':                           // unix code
            return 1;
        default:                     
            return 0;
    }
}

int hex_char_to_decimal(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return 10 + (c - 'A');
    } else if (c >= 'a' && c <= 'f') {
        return 10 + (c - 'a');
    }
    return -1; // Invalid hex character
}

int unicode(int index, Token* token) {
    char curr = getchar();  // Get the first hex digit
    int unicodeValue = 0;

    for (int i = 0; i < 2; i++) {

        // Check if valid hex character and convert to decimal
        int hexValue = hex_char_to_decimal(curr);
        if (hexValue == -1) {
            return -1;  // Error in case of invalid hex digit
        }

        // Accumulate the hex value (shift left by 4 bits to make room for next digit)
        unicodeValue = (unicodeValue << 4) | hexValue;

        // Read the next character
        curr = getchar();
    }
    ungetc(curr, stdin);  // Can change the structure of code so it doenst read additional char
    token->value.stringVal[index] = (char) unicodeValue;
    return unicodeValue;  // Return the converted decimal value
}

Token* String(){
    Token* token = Token_create(T_String, TC_VALUE);
    int length=60;
    token->value.stringVal = malloc(sizeof(char) * length);
    if (token->value.stringVal == NULL) {                 // malloc failed
        free(token->value.stringVal);
        create_error(token,99);
        return token;
    }

    int index = 0;
    char Char;
    Char = getchar();
    int good = 1;
    while(1) {
        //printf("%c\n",Char);
        if (Char == '\n' || Char == EOF ){     // Errors
            create_error(token,1);
            return token;
        }

        if (index >= length - 10) {  
            token->value.stringVal = realloc(token->value.stringVal, length * 2);
            length = length * 2;
            if (token->value.stringVal == NULL) {
                create_error(token,99);
                return token;
            }
        }

        if (Char == '"'){     // end of string
            break;
        }
        else if (Char == '\\') {
            int escape = escape_sequence();
            if (escape == 0) {
                create_error(token,1);
                return token;
            }
            else if (escape == 1) {
                good = unicode(index, token);
                index++;
                if (good == -1) {
                    create_error(token,1);
                    return token;
                }
                //printf("%caaaaa\n",Char);
            }
            else {
                token->value.stringVal[index] = escape;
                index++;
            }
            Char = getchar();
        }
        
        else {                                  // char in automata
            token->value.stringVal[index] = Char;
            index++;
            Char = getchar();
        }
    }
    token->value.stringVal[index] = '\0';
    return token;
}

Token* multiline_string() {
    Token* token = Token_create(T_String, TC_VALUE);
    int length = 60;
    token->value.stringVal = malloc(sizeof(char) * length);

    if (token->value.stringVal == NULL) {  // malloc failed
        create_error(token, 99);
        return token;
    }

    int index = 0;
    char Char;
    bool can_read = true;

    // Check for the second Initial Backslash
    Char = getchar();
    if (Char != '\\') {
        create_error(token, 1);
        free(token->value.stringVal);
        return token;
    }

    while (1) {
        Char = getchar();

        // Reallocate if running out of space
        if (index >= length - 10) {  // reserve space for 
            char *temp = realloc(token->value.stringVal, length * 2);
            if (temp == NULL) {
                free(token->value.stringVal);  // free original memory
                create_error(token, 99);
                return token;
            }
            token->value.stringVal = temp;
            length *= 2;
        }

        // End if ; but also needs to if it is on alonely line
        if (Char == ';' && can_read == false) {
            break;
        }

        // Error if end of file 
        if (Char == EOF) {
            create_error(token, 1);
            return token;
        }

        // Stil reading the same row (can read anythind not[\n EOF])
        if (can_read) {
            if (Char == '\n') {
                can_read = false;
                token->value.stringVal[index++] = '\n';  // Append newline character
            } 
            else {
                token->value.stringVal[index++] = Char;
            }
        }

        // looking for begginig of new line 
        else if (!can_read) {
            if (Char == '\\') {
                Char = getchar();
                if (Char == '\\') {
                    can_read = true;  // Set can_read for next line
                } 
                else {
                    create_error(token, 1);
                    free(token->value.stringVal);
                    return token;
                }
            } 
        }
    }

    
    token->value.stringVal[index-2] = '\0';  // remove the last newline as its not part of intended string
    return token;
}

Token* scan() {                             
    char curr = GetNotWhiteChar();         
    char next = curr;
    Token* token;
    //printf("%c \n",curr);

    switch (curr) {
        case '-':
            token = Token_create(T_Minus, TC_OPERATOR);
            break;

        case '+':
            token = Token_create(T_Plus, TC_OPERATOR);
            break;

        case '*':
            token = Token_create(T_Mul, TC_OPERATOR);
            break;

        case '/':       // can be comment
            if ((next = getchar()) == '/') {
                SkipComment();
                //printf("Comment skipped\n");
                return scan();                  // found comment so we need to call scan again so it doesnt return previous token
            }
            else {
                ungetc(next, stdin);
                token = Token_create(T_Div, TC_OPERATOR);
            }
            break;
            
        case '=':
            if ((next = getchar()) == '=') {                 // ==
                token = Token_create(T_Equal, TC_LOGICAL);
            } 
            else {
                token = Token_create(T_Assign, TC_OPERATOR);
                ungetc(next, stdin);
            }
            break;

        case '<':
            if ((next = getchar()) == '=') {                 // <=
                token = Token_create(T_Less_Eq, TC_LOGICAL);
            } 
            else {
                token = Token_create(T_Lesser, TC_LOGICAL);
                ungetc(next, stdin);
            }
            break;

        case '>':
            if ((next = getchar()) == '=') {                 // >=
                token = Token_create(T_Great_Eq, TC_LOGICAL);
            } 
            else {
                token = Token_create(T_Greater, TC_LOGICAL);
                ungetc(next, stdin);
            }
            break;

        case '!':
            if ((next = getchar()) == '=') {                 // !=
                token = Token_create(T_Not_Eq, TC_LOGICAL);
            } 
            else {
                token = Token_create(T_ERORR, TC_ERR);
                token->value.code=1;
            }
            break;
        
        case '(':
            token = Token_create(T_L_Round_B, TC_BRACKET);
            break;
        
        case ')':
            token = Token_create(T_R_Round_B, TC_BRACKET);
            break;
        
        case '[':
            token = Token_create(T_L_Square_B, TC_BRACKET);
            break;
        
        case ']':
            token = Token_create(T_R_Square_B, TC_BRACKET);
            break;
        
        case '{':
            token = Token_create(T_L_Curly_B, TC_BRACKET);
            break;
        
        case '}':
            token = Token_create(T_R_Curly_B, TC_BRACKET);
            break;

        case '%':
            token = Token_create(T_Percent, TC_PUNCATION);
            break;

        case ';':
            token = Token_create(T_SemiC, TC_PUNCATION);
            break;
        
        case ',':
            token = Token_create(T_Comma, TC_PUNCATION);
            break;

        case '.':
            token = Token_create(T_Dot, TC_PUNCATION);
            break;

        case '@':
            token = Token_create(T_At, TC_PUNCATION);
            break;

        case ':':
            token = Token_create(T_Colon, TC_PUNCATION);
            break;

        case '?':
            token = Token_create(T_Question, TC_PUNCATION);
            break;

        case '|':
            token = Token_create(T_Pipe, TC_PUNCATION);
            break;

        case '_':
            next = getchar();
            if (isalnum(next) || next=='_') {
                token = Create_ID_or_Keyword(curr);
                ungetc(next, stdin);
                return token;                   
            }

            else {
                ungetc(next, stdin);
                return Token_create(T_Underscore, TC_PUNCATION);
            }
            break;

        case 'a': case 'b': case 'c': case 'd': case 'e':case 'f': case 'g': case 'h': case 'i': case 'j':case 'k': case 'l': case 'm': case 'n': case 'o':
        case 'p': case 'q': case 'r': case 's': case 't':case 'u': case 'v': case 'w': case 'x': case 'y':case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E':case 'F': case 'G': case 'H': case 'I': case 'J':case 'K': case 'L': case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R': case 'S': case 'T':case 'U': case 'V': case 'W': case 'X': case 'Y':case 'Z':
            token = Create_ID_or_Keyword(curr);
            break;
        
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            token = numbers(curr);
            break;
        
        case '"':
            token = String();
            break;
        case '\\':
            token = multiline_string();
            break;

        case EOF:
            token = Token_create(T_EOF, TC_EOF);
            break;
        

        default:
            token = Token_create(T_ERORR, TC_ERR);
            token->value.code=1;
            break;
    }
    return token;
}


// testings
/*
const char* token_type_names[] = {
    "T_Minus", "T_Plus", "T_Mul", "T_Div", "T_Assign",
    "T_Equal", "T_Lesser", "T_Less_Eq", "T_Greater", "T_Great_Eq", "T_Not_Eq",
    "T_L_Square_B", "T_R_Square_B", "T_L_Curly_B", "T_R_Curly_B", "T_L_Round_B", "T_R_Round_B",
    "T_Percent", "T_SemiC", "T_Comma", "T_Dot", "T_At", "T_Colon", "T_Question", "T_Pipe", "T_Underscore",
    "T_const", "T_else", "T_fn", "T_if", "T_i32", "T_f64", "T_null", "T_pub", "T_return", "T_u8", "T_var", "T_void", "T_while", "T_import",
    "T_ID",
    "T_Integer", "T_Float", "T_Exponent", "T_String",
    "T_EOF", "T_ERORR"
};

int main() {                                    
     Token* token;
     token = scan();
     printf("Token type: %s\n", token_type_names[token->type]);
     while (token->type != T_EOF) {
        token = scan();
        if (token->Category == TC_ERR) {
            ThrowError(token->value.code);
            break;
         }
        else {
            printf("Token type: %s\n", token_type_names[token->type]);
        }
        if (token->Category == TC_VALUE) {
            if (token->type == T_String) {
                printf("Value: \n|%s|\n", token->value.stringVal);
            }
            else if (token->type == T_Float) {
                printf("Value: %f\n", token->value.Float);
            }
            else if (token->type == T_Integer) {
                printf("Value: %d\n", token->value.integer);
            }
            else if (token->type == T_ID){
                printf("Value: %s\n", token->value.ID_name);
            }
        }
    }
    return 0;
    }
*/
