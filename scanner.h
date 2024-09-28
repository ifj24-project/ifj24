#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum token_Category {
    TC_OPERATOR,
    TC_LOGICAL,
    TC_BRACKET,
    TC_PUNCATION,
    TC_KEYWORDS,
    TC_ID,
    TC_VALUE,
    TC_ERR,
    TC_EOF
}token_Category;

enum token_type {
    //operators         (ASCII) {State} (index)
    T_Minus,            // -     {F1}    (0)
    T_Plus,             // +     {F2}    (1)
    T_Mul,              // *     {F3}    (2)
    T_Div,              // /     {F4}    (3)
    T_Assign,           // =     {F5}    (4)

    // logical
    T_Equal,            // ==   {F6}    (5)
    T_Lesser,           // <    {F7}    (6)
    T_Less_Eq,          // <=   {F8}    (7)
    T_Greater,          // >    {F9}    (8)
    T_Great_Eq,         // >=   {F10}   (9)
    T_Not_Eq,           // !=   {F11}   (10)

    // Brackets
    T_L_Square_B,       // [    {F12}   (11)
    T_R_Square_B,       // ]    {F13}   (12)
    T_L_Curly_B,        // {    {F14}   (13)
    T_R_Curly_B,        // }    {F15}   (14)
    T_L_Round_B,        // (    {F16}   (15)
    T_R_Round_B,        // )    {F17}   (16)

    // Punctation
    T_Percent,          // %    {F18}   (17)
    T_SemiC,            // ;    {F19}   (18)
    T_Comma,            // ,    {F20}   (19)
    T_Dot,              // .    {F21}   (20)
    T_At,               // @    {F22}   (21)
    T_Colon,            // :    {F23}   (22)
    T_Question,         // ?    {F24}   (23)
    T_Underscore,       // _    {F25}   (24)

    // Keywords                 {F26}
    T_const,            // const        (25)
    T_else,             // else         (26)
    T_fn,               // fn           (27)
    T_if,               // if           (28)
    T_i32,              // i32          (29)
    T_f64,              // f64          (30)
    T_null,             // null         (31)
    T_pub,              // pub          (32)
    T_return,           // return       (33)
    T_u8,               // u8           (34)
    T_var,              // var          (35)
    T_void,             // void         (36)
    T_while,            // while        (37)
    T_import,           // import       (38)  

    // ID                       
    T_ID,               // Id   {F26}    (39)

    // Value
    T_Integer,          // Integer  {f28}   (40)
    T_Float,            // Float    {f29}   (41)
    T_Exponent,         // Exponent {f30}   (42)
    T_String,           // String   {f31}   (43)

    // Errors
    T_EOF,              // End of file  {F32}   (44)
    T_ERORR             // Error                (45)

}token_type;

typedef struct tokenValue_t{
    char* ID_name;
    char* stringVal;
    int integer;
    double Float;
    int code;               // error code 
    bool nullable;           // nullable 
} tokenValue_t;


typedef struct Token {
    enum token_type type;
    enum token_Category Category;
    tokenValue_t value;
} Token;

