#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"

/* TODO :
    - Add keywords
    - Add values

    - How long Identifiers can be ?
    - Nullables
    - Param,import as KW 
    
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
        token->Category = TC_ERR;
        token->type = T_ERORR;
        token->value.code=99;
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
        else {                                  // char is automata
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
            // 38 is the first keyword, so I flipped list of keywords and minused the position of the index
            token->type = 38 - i;
            return token;
        }
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

const char* token_type_names[] = {
    "T_Minus", "T_Plus", "T_Mul", "T_Div", "T_Assign",
    "T_Equal", "T_Lesser", "T_Less_Eq", "T_Greater", "T_Great_Eq", "T_Not_Eq",
    "T_L_Square_B", "T_R_Square_B", "T_L_Curly_B", "T_R_Curly_B", "T_L_Round_B", "T_R_Round_B",
    "T_Percent", "T_SemiC", "T_Comma", "T_Dot", "T_At", "T_Colon", "T_Question", "T_Underscore",
    "T_const", "T_else", "T_fn", "T_if", "T_i32", "T_f64", "T_null", "T_pub", "T_return", "T_u8", "T_var", "T_void", "T_while", "T_import",
    "T_ID",
    "T_Integer", "T_Float", "T_Exponent", "T_String",
    "T_EOF", "T_ERORR"
};

int main() {                                    
     Token* token;
     token = scan();
     while (token->type != T_EOF) {
         if (token->Category == TC_ERR) {
            printf("Error code: %d\n", token->value.code);
             break;
         }
         else {
             printf("Token type: %s\n", token_type_names[token->type]);
         }
         token = scan();
    }
    printf("Token type: %s\n", token_type_names[token->type]);
    return 0;
    }
 
