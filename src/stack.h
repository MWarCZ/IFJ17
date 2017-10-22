//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/stack.c                 //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef STACK_H
#define STACK_H

/**
 * @brief Struktura predstavujici data polozky zasobniku
 */
typedef struct TStackData {
  int i;
} TStackData;

/**
 * @brief Struktura predstavujici polozku zasobniku
 */
typedef struct TStackItem {
  TStackData data; //Data
  struct TStackItem *next; //Ukazatel na dalsi polozku
} TStackItem;

/**
 * @brief Struktura predstavujici zasobnik
 */
typedef struct TStack {
  TStackItem *top; //Ukazatel na vrchol zasobniku
  int count; //Aktualni pocet polozek na zasobniku
} TStack;


/**
 * @brief Inicializuje zasobnik
 * @return Ukazatel na inicializovanou strukturu TStack
 */
TStack *StackInit();

/**
 * @brief Ulozi data na vrchol zasobniku
 * @param data Struktura TStackData, ktera bude ulozena na zasobnik
 * @return Hodnotu 1 pri uspechu, hodnotu 0 pri chybe
 */
int StackPush(TStack *stack, TStackData data);

/**
 * @brief Odstrani data z vrcholu zasobniku a vrati je
 * @param stack Ukazatel na zasobnik
 * @param data Ukazatel na strukturu TStackData kam budou ulozena data ze zasobniku
 * @return Hodnotu 1 pri uspechu, hodnotu 0 pri chybe (zasobnik je prazdny)
 */
int StackPop(TStack *stack, TStackData *data);

/**
 * @brief Odstrani zasobnik a jeho polozky
 * @param stack Ukazatel na zasobnik
 */
void StackDestroy(TStack *stack);

/**
 * @brief Odstrani vsechny polozky zasobniku
 * @param stack Ukazatel na zasobnik
 */
void StackEmpty(TStack *stack);

#endif

//Soubor src/stack.h