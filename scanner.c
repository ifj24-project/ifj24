#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"

/* TODO :
    - Skip Comments
    - Add keywords
    - Add identifiers
    - Add values
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

Token* Token_create(enum token_type type, enum token_Category category) {
    Token* token = malloc(sizeof(Token)); 
    if (token == NULL) {
        token->Category = TC_ERR;
        token->type = T_ERORR;
        token->value.code=99;
    }
    else{
        token->type = type;
        token->Category = category;
    }
    return token;
}

Token* scan() {                             
    char curr = GetNotWhiteChar();         
    char next = curr;
    Token* token;
    //printf("%c",curr);

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
            token = Token_create(T_Div, TC_OPERATOR);
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

        case '_':
            token = Token_create(T_Underscore, TC_PUNCATION);
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

// int main() {                                    
//     Token* token;
//     token = scan();
//     while (token->type != T_EOF) {
//         if (token->Category == TC_ERR) {
//             printf("Error code: %d\n", token->value.code);
//             break;
//         }
//         else {
//             printf("Token type: %d\n", token->type);
//         }
//         token = scan();
//     }
//     return 0;
//     }
 
