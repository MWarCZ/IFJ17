//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/dstring.h                //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef DSTRING_H
#define DSTRING_H

#define ALLOC_STEP 50

typedef struct TString {
    char *string;
    unsigned int length;
    unsigned int allocLength;
} TString;

/**
 * @brief Inicializuje retezec
 * @return Ukazatel na inicializovany retezec, nebo NULL pri neuspechu
 */
TString *StringInit();

/**
 * @brief Priradi do retezce C-string
 * @param s Ukazatel na retezec
 * @param value C retezec s hodnotou pro prirazeni
 * @return 1 pri uspechu, 0 pri chybe
 */
int StringAssign(TString *s, char *str);

/**
 * @brief Prida znak na konec retezce
 * @param s Ukazatel na retezec
 * @param c Znak pro pridani
 * @return 1 pri uspechu, 0 pri chybe
 */
int StringAdd(TString *s, char c);

/**
 * @brief Odstrani znak z konce retezce
 * @param s Ukazatel na retezec
 * @return Znak, ktery byl z retezce odstranen, nebo EOF pokud je retezec prazdny
 */
int StringPop(TString *s);

/**
 * @brief Vyprazdni retezec
 * @param s Ukazatel na retezec
 */
void StringEmpty(TString *s);

/**
 * @brief Spravne odstrani retezec
 * @param s Ukazatel na retezec
 */
void StringDestroy(TString *s);

#endif