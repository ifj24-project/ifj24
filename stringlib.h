/** 
* @file stringlib.h
* @author Ariana Tomen (xtomen00)
*
* IFJ24
*
* @brief Definice struktury String a funkci pro implementaci knihovny
*/

#ifndef STRINGLIB_H
#define STRINGLIB_H

#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Struktura reprezentujici dynamicky alokovany retezec.
 */
typedef struct {
    char* data;       /**< Ukazatel na pole znaku obsahujici data retezce. */
    size_t length;    /**< Aktualni delka retezce (bez znakoveho terminatoru). */
    size_t capacity;  /**< Celkova kapacita alokovane pameti pro retezec. */
} String;

/**
 * @brief Vytvori prazdny retezec s pocatecni kapacitou.
 * 
 * Alokuje pamet pro novou strukturu retezce a inicializuje ji jako prazdny retezec.
 * 
 * @return String* Ukazatel na novy prazdny retezec.
 * @throws 99 Pokud alokace pameti selze.
 */
String* create_empty_string();

/**
 * @brief Vytvori retezec inicializovany zadanymi daty.
 * 
 * Alokuje pamet pro novou strukturu retezce a zkopiruje do ni poskytnuta data.
 * 
 * @param initial_data Ukazatel na puvodni znaky pro retezec.
 * @return String* Ukazatel na novy retezec.
 * @throws 99 Pokud alokace pameti selze.
 */
String* create_string(const char *initial_data);

/**
 * @brief Uvolni pamet spojenu s retezcem.
 * 
 * Uvolni pamet pouzitou pro strukturu retezce a jeho data.
 * 
 * @param str Ukazatel na retezec, ktery bude uvolnen.
 */
void free_string(String* str);

/**
 * @brief Uvolni pamet spojenu s retezcem.
 * 
 * Uvolni pamet pouzitou pro strukturu retezce a jeho data.
 * 
 * @param str Ukazatel na retezec, ktery bude uvolnen.
 */
String* copy_string(const String *str);

/**
 * @brief Porovna dva retezce.
 * 
 * Provede lexikograficke porovnani dvou retezcu.
 * 
 * @param str1 Ukazatel na prvni retezec.
 * @param str2 Ukazatel na druhy retezec.
 * @return int Vraci 0, pokud jsou retezce stejne, jinak 1.
 * @throws 99 Pokud je nejaky z retezcu NULL.
 */
int compare_strings(const String* str1, const String* str2);

/**
 * @brief Spoji dva retezce do noveho retezce.
 * 
 * Alokuje pamet pro novy retezec obsahujici spojeni dvou puvodnich retezcu.
 * 
 * @param str1 Ukazatel na prvni retezec.
 * @param str2 Ukazatel na druhy retezec.
 * @return String* Ukazatel na novy retezec.
 * @throws 99 Pokud alokace pameti selze nebo nejaky z retezcu je NULL.
 */
String* concat_strings(const String *str1, const String *str2);

/**
 * @brief Zmeni kapacitu retezce.
 * 
 * Zvysi kapacitu retezce, pokud nova kapacita je vetsi nez soucasna kapacita.
 * 
 * @param str Ukazatel na retezec, jehoz kapacitu chceme zmenit.
 * @param new_capacity Nova pozadovana kapacita retezce.
 */
void resize_string(String* str, size_t new_capacity);

/**
 * @brief Vytvori podretezec z daneho retezce.
 * 
 * Alokuje pamet pro novy retezec obsahujici podretezec od startovaciho do konecneho indexu.
 * 
 * @param str Ukazatel na puvodni retezec.
 * @param start Zacatecni index podretezce (vcetne).
 * @param end Konecny index podretezce (vylucne).
 * @return String* Ukazatel na novy podretezec.
 * @throws 99 Pokud alokace pameti selze.
 * @throws 4 Pokud jsou indexy neplatne nebo mimo rozsah.
 */
String* substring_string(const String *str, size_t start, size_t end);

#endif
