//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/symtable.h               //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////
#include "error.h"
#include "token.h"
#include "dstring.h"
#include "token.h"

#ifndef SYMTABLE_H
#define SYMTABLE_H

/**
 * @brief Struktura popisující prvek tabulky
 * 
 * 
 * 
**/
typedef struct htab_LI{
    struct htab_LI* next;   //ukazatel na následující prvek
    char key[];             //název identifikátoru
    TToken *token;          //data o daném symbolu (tokenu)
}htab_listItem;

/**
 * @brief Struktura popisující hashovací tabulku
 * 
 * 
 * */
typedef struct htab{
    unsigned htab_size;     //velikost tabulky
    struct htab_LI* list[]; //pole tabulky
} htab_t;

/**
 * @brief Rozptylovací funkce
 * */
unsigned hash_function(char *str);
/**
 * @brief Vytvoří a inicializuje tabulku
 * */
htab_t *htab_init(unsigned size);
/**
 * @brief Hleda v tabulce podle zadaného klíče
 * 
 * */
htab_listItem *htab_find(htab_t *t, char *key);
/**
 * @brief Vytváří prvek seznamu
 * */
htab_listItem *htab_create(htab_t *t, char *key);
/**
 * @brief Provádí funkci pro každý prvek
 * */
void htab_foreach(htab_t *t, void (*function)(char *key, ??,htab_listItem *item));
/**
 * @brief odstraní prvek podle zadaného klíče
 * */
bool htab_remove(htab_t *t,const char *key);
/**
 * @brief Zruší všechny prvky tabulky a tabulka zůstane prázdná
 * 
 * */
void htab_clear(htab_t *t);
/**
 * @brief Zruší celou tabulku
 * */
void htab_free(htab_t *t);

#endif

//  src/symtable.h