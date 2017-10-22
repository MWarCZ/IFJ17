//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/stack.c                  //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef STACK_C
#define STACK_C

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

TStack *StackInit() {
  TStack *stack = NULL;

  if ((stack = malloc(sizeof(TStack))) == NULL) {
    return NULL;
  }

  stack->count = 0;
  stack->top = NULL;

  return stack;
}

int StackPush(TStack *stack, TStackData data) {
  TStackItem *item = NULL;

  //Vytvorit polozku
  if ((item = malloc(sizeof(TStackItem))) == NULL) {
    return 0;
  }

  item->data = data;
  item->next = NULL;

  //Pridat polozku na vrchol
  stack->count++;

  if (stack->top == NULL) {
    stack->top = item;
  } else {
    item->next = stack->top;
    stack->top = item;
  }

  return 1;
}

int StackPop(TStack *stack, TStackData *data) {
  if (stack->top == NULL) {
    //Zasobnik je prazdny
    return 0;
  } else {
    (*data) = stack->top->data;
    TStackItem *next = stack->top->next;

    free(stack->top);
    stack->top = next;
    stack->count--;

    return 1;
  }
}

void StackDestroy(TStack *stack) {
  //Uvolni polozky
  for (TStackItem *item = stack->top; item != NULL; item = item->next) {
    free(item);
  }

  //Uvolni zasobnik
  free(stack);
}

#endif

//Soubor src/stack.c
