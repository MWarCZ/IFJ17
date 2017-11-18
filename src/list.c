//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/list.c                   //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef LIST_C
#define LIST_C

#include <stdio.h>
#include <stdlib.h>
#include "list.h"


/*
 * Interni funkce, ktere je treba upravit pri kazde zmene struktury TListData 
 */

//Vraci 0 nebo 1 podle toho jestli se struktury TListData rovnaji
int CompareData(TListData data1, TListData data2) {
  return data1.i == data2.i;
}

//Tiskne data, ktera se zobrazi pri pouziti funkce ListPrint
void PrintData(TListData data) {
  printf("%d", data.i);
}

/**********************************************************************/


TList *ListInit() {
  TList *list = NULL;

  if ((list = malloc(sizeof(TList))) == NULL) {
    return NULL;
  }

  list->count = 0;
  list->front = list->back = NULL;

  return list;
}

TList *ListCopy(TList *list) {
  TList *newList;

  if ((newList = ListInit()) == NULL) {
    return NULL; //Vytvoreni noveho seznamu selhalo
  }

  //Zkopiruje polozky
  for (TListItem *item = list->front; item != NULL; item = item->next) {
    if (!ListPushBack(newList, item->data)) {
      ListDestroy(newList);
      return NULL; //Pridani polozky selhalo
    }
  }

  return newList;
}

void ListClear(TList *list) {
  for (TListItem *item = list->front; item != NULL; item = item->next) {
    if (item->prev != NULL) {
      free(item->prev); //Uvolni vzdy predchozi prvek aktualniho prvku
    }
  }

  free(list->back); //Uvolni posledni prvek

  list->front = list->back = NULL;
  list->count = 0;
}

void ListDestroy(TList *list) {
  //Uvolni polozky
  ListClear(list);

  //Uvolni zasobnik
  free(list);
}

void ListPrint(TList *list) {
  if (ListEmpty(list)) {
    printf("[]\n");
  } else {

    int i = 0;
    for (TListItem *item = list->front; item != NULL; item = item->next) {
      printf("[%d]\t[\t", i++);
      
      if (item->prev != NULL) {
        PrintData(item->prev->data);
      } else {
        printf("NULL");
      }
      
      printf("\t<-\t");
      PrintData(item->data);
      printf("\t->\t");
  
      if (item->next != NULL) {
        PrintData(item->next->data);
      } else {
        printf("NULL");
      }
  
      printf("\t]");
  
      if (item == list->front) {
        printf("\t<- front");
      }
      if (item == list->back) {
        printf("\t<- back");
      }
  
      printf("\n");
    }

    printf("List length: %d\n", list->count);
  }
}


int ListPush(TList *list, TListData data) {
  TListItem *item = NULL;

  //Vytvori polozku
  if ((item = malloc(sizeof(TListItem))) == NULL) {
    return 0; //Alokace pameti selhala 
  }

  item->data = data;
  item->next = list->front;
  item->prev = NULL;

  //Je seznam prazdny?
  if (ListEmpty(list)) {
    list->back = item; //Seznam je prazdny, vkladany prvek je zaroven i posledni
  } else {
    list->front->prev = item; //Seznam neni prazdny, nastavime predchozi prvek byvalemu prvnimu
  }

  list->front = item;
  list->count++;

  return 1;
}

int ListPop(TList *list, TListData *data) {
  if (ListEmpty(list)) {
    return 0; //Seznam je prazdny
  } else {
    if (data != NULL) {
      (*data) = list->front->data; //Vrati data pokud je dodan ukazatel
    }
    TListItem *oldFront = list->front; //Ulozime stary prvni prvek kvuli free

    //Smazali jsme posledni prvek v seznamu?
    if ((list->front = list->front->next) == NULL) {
      list->back = NULL; //Seznam je prazdny, nastavime i posledni prvek na NULL
    } else {
      list->front->prev = NULL; //Nastavime prechozi prvek prvniho prvku na NULL
    }

    free(oldFront);
    list->count--;

    return 1;
  }
}

int ListPushBack(TList *list, TListData data) {
  TListItem *item = NULL;
  
  //Vytvori polozku
  if ((item = malloc(sizeof(TListItem))) == NULL) {
    return 0; //Alokace pameti selhala 
  }

  item->data = data;
  item->next = NULL;
  item->prev = list->back;

  //Je seznam prazdny?
  if (ListEmpty(list)) {
    list->front = item; //Seznam je prazdny, vkladany prvek je zaroven i prvni
  } else {
    list->back->next = item; //Seznam neni prazdny, nastavime nasledujici prvek byvalemu poslednimu
  }

  list->back = item;
  list->count++;

  return 1;
}

int ListPopBack(TList *list, TListData *data) {
  if (ListEmpty(list)) {
    return 0; //Seznam je prazdny
  } else {
    if (data != NULL) {
      (*data) = list->back->data; //Vrati data pokud je dodan ukazatel
    }
    TListItem *oldBack = list->back; //Ulozime stary posledni kvuli free

    //Smazali jsme posledni prvek v seznamu?
    if ((list->back = list->back->prev) == NULL) {
      list->front = NULL; //Seznam je prazdny, nastavime i prvni prvek na NULL
    } else {
      list->back->next = NULL; //Nastavime nasledujici prvek posledniho prvku na NULL
    }

    free(oldBack);
    list->count--;

    return 1;
  }
}

int ListFront(TList *list, TListData *data) {
  if (ListEmpty(list)) {
    return 0;
  } else {
    (*data) = list->front->data;
    return 1;
  }
}

int ListBack(TList *list, TListData *data) {
  if (ListEmpty(list)) {
    return 0;
  } else {
    (*data) = list->back->data;
    return 1;
  }
}


int ListInsert(TList *list, int index, TListData data) {
  if (index > list->count || index < 0) {
    return 0; //Index je mimo hranice seznamu
  } else if (index == 0) {
    ListPush(list, data); //Vkladame na prvni misto
  } else if (index == list->count) {
    ListPushBack(list, data); //Vkladame na posledni misto
  } else {
    TListItem *item; //Vkladana polozka
    TListItem *current = list->front; //Polozka aktualne na danem indexu

    //Vytvori novou polozku
    if ((item = malloc(sizeof(TListItem))) == NULL) {
      return 0; //Alokace pameti selhala
    }

    //Najde polozku, ktera se aktualne na danem indexu nachazi
    for (int i = 0; i < index; i++) {
      current = current->next;
    }

    //Vlozi polozku na dany index
    item->data = data;
    item->prev = current->prev;
    item->next = current;
    
    current->prev->next = item;
    current->prev = item;


    list->count++;
  }
  
  return 1;
}

int ListRemove(TList *list, int index, TListData *data) {
  if (index >= list->count || index < 0) {
    return 0; //Index je mimo hranice seznamu
  } else if (index == 0) {
    ListPop(list, data);
  } else if (index == list->count - 1) {
    ListPopBack(list, data);
  } else {
    TListItem *item = list->front;

    //Najde mazanou polozku
    for (int i = 0; i < index; i++) {
      item = item->next;
    }

    //Vrati data pokud je dodan ukazatel
    if (data != NULL) {
      (*data) = item->data;
    }

    item->prev->next = item->next;
    item->next->prev = item->prev;

    free(item);
    list->count--;
  }

  return 1;
}

int ListGet(TList *list, int index, TListData *data) {
  if (ListEmpty(list) || index >= list->count || index < 0) {
    return 0; //Index je mimo hranice seznamu
  } else {
    TListItem *item = list->front;

    for (int i = 0; i < index; i++) {
      item = item->next;
    }

    (*data) = item->data;

    return 1;
  }
}


int ListEmpty(TList *list) {
  return list->count == 0;
}

int ListCompare(TList *list1, TList *list2) {
  if (list1->count == list2->count) {
    if (list1->count == 0) {
      return 1; //Oba dva seznamy jsou prazdne
    } else {
      TListItem *item1 = list1->front;
      TListItem *item2 = list2->front;
      
      //Zkontroluje jestli maji vsechny polozky stejna data
      do {
        if (!CompareData(item1->data, item2->data)) {
          return 0;
        }
      } while ((item1 = item1->next) != NULL && (item2 = item2->next) != NULL);

      return 1;
    }
  } else {
    return 0; //Seznamy maji rozdilnou delku
  }
}

int ListCompareLength(TList *list1, TList *list2) {
  if (list1->count == list2->count) {
    return 0; //Shodne
  } else if (list1->count > list2->count) {
    return 1; //List1 je delsi
  } else {
    return -1; //List2 je delsi
  }
}

#endif

//Soubor src/list.c
