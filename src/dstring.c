//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/dstring.c                //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef DSTRING_C
#define DSTRING_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dstring.h"

TString *StringInit() {
  TString *s;

  //Alokuje retezec
  if ((s = malloc(sizeof(TString))) == NULL) {
    return NULL; //Alokace pameti selhala
  }

  //Alokuje prvnich ALLOC_STEP znaku pro retezec
  if ((s->string = malloc(ALLOC_STEP * sizeof(char))) == NULL) {
    free(s);     //Uvolni alokovanou strukturu
    return NULL; //Alokace pameti selhala
  }

  s->allocLength = ALLOC_STEP;
  StringEmpty(s); //Vyprazdni retezec

  return s;
}

int StringAdd(TString *s, char c) {
  //Je potreba alokovat nove misto?
  if (s->length == s->allocLength - 1) {
    s->allocLength += ALLOC_STEP; //Nastavi novou velikost
    
    //Realokuje retezec
    if ((s->string = realloc(s->string, s->allocLength * sizeof(char))) == NULL) {
      //Realokace selhala
      s->allocLength = s->length = 0;
      return 0;
    }
  }

  s->string[s->length++] = c; //Prida znak do retezce a zvetsi delku retezce o 1
  s->string[s->length] = '\0'; //Posune null terminator

  return 1;
}


int StringPop(TString *s) {
  if (s->length == 0) {
    return EOF; //Retezec je prazdny
  } else {
    char c = s->string[--s->length]; //Ulozi posledni znak a zmensi delku retezce o 1
    s->string[s->length] = '\0'; //Posune null terminator
    
    return c;
  }
}

int StringAssign(TString *s, char *str) {  
  //Zvetsi dynamicky retezec pokud je potreba (- 1 kvuli null terminatoru)
  if (strlen(str) > s->allocLength - 1) {
    //Nastavi potrebnou velikost dynamickeho retezce
    while (strlen(str) > s->allocLength - 1) {
      s->allocLength += ALLOC_STEP;
    }
    
    //Realokuje retezec
    if ((s->string = realloc(s->string, s->allocLength * sizeof(char))) == NULL) {
      //Realokace selhala
      s->allocLength = s->length = 0;
      return 0;
    } 
  }
  
  StringEmpty(s); //Vyprazdni retezec
  strcpy(s->string, str); //Zkopiruje retezec
  s->length = strlen(str); //Nastavi delku

  return 1;
}

void StringEmpty(TString *s) {
  memset(s->string, '\0', s->allocLength); //Nastavi vsechny bajty retezce na \0
  s->length = 0;
}

void StringDestroy(TString *s) {
  free(s->string);
  free(s);
}

#endif