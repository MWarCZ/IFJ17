//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/list.h                   //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef LIST_H
#define LIST_H

/**
 * @brief Struktura predstavujici data polozky zasobniku
 * @warning Prepsani teto struktury vyzaduje take upravy internich funkci PrintData a CompareData
 */
typedef struct TListData {
  int i;
} TListData;

/**
 * @brief Struktura predstavujici polozku zasobniku
 */
typedef struct TListItem {
  struct TListItem *next; //Ukazatel na dalsi polozku
  struct TListItem *prev; //Ukazatel na predchozi polozku
  TListData data; //Data
} TListItem;

/**
 * @brief Struktura predstavujici zasobnik
 */
typedef struct TList {
  TListItem *front; //Ukazatel na prvni prvek
  TListItem *back; //Ukazatel na posledni prvek
  int count; //Aktualni pocet polozek v seznamu
} TList;


/**
 * @brief Inicializuje seznam
 * @return Ukazatel na inicializovanou strukturu TList
 */
TList *ListInit();

/**
 * @brief Vytvori hlubokou kopii existujiciho seznamu
 * @param list Ukazatel na seznam, ktery bude zkopirovan
 * @return Ukazatel na nove vytvoreny seznam
 */
TList *ListCopy(TList *list);

/**
 * @brief Odstrani vsechny polozky seznam
 * @param stack Ukazatel na seznam
 */
void ListClear(TList *list);

/**
 * @brief Odstrani seznam a jeho polozky
 * @param stack Ukazatel na seznam
 */
void ListDestroy(TList *list);

/**
 * @brief Vytiskne reprezentaci daneho seznamu
 * @param list Ukazatel na seznam
 */
void ListPrint(TList *list);


/**
 * @brief Vlozi data na zacatek seznamu
 * @param list Ukazatel na seznam
 * @param data Struktura TListData, ktera bude vlozena
 * @return Hodnotu 1 pri uspechu, hodnotu 0 pri chybe
 */
int ListPush(TList *list, TListData data);

/**
 * @brief Odstrani data ze zacatku seznamu
 * @param stack Ukazatel na zasobnik
 * @param data Ukazatel na strukturu TListData kam budou ulozena data
 * @return Hodnotu 1 pri uspechu, hodnotu 0 pri chybe (seznam je prazdny)
 */
int ListPop(TList *list, TListData *data);

/**
 * @brief Vlozi data na konec seznamu
 * @param list Ukazatel na seznam
 * @param data Struktura TListData
 * @return Hodnotu 1 pri uspechu, hodnotu 0 pri chybe
 */
int ListPushBack(TList *list, TListData data);

/**
 * @brief Odstrani data z konce seznamu
 * @param list Ukazatel na seznam
 * @param data Ukazatel na strukturu TListData kam budou ulozena data
 * @return Hodnotu 1 pri uspechu, 0 pri chybe (zasobnik je prazdny)
 */
int ListPopBack(TList *list, TListData *data);

/**
 * @brief Vrati data ze zacatku seznamu
 * @param list Ukazatel na seznam
 * @param data Ukazatel na strukturu TListData kam budou ulozena data
 * @return Hodnotu 1 pri uspechu, 0 pri chybe (zasobnik je prazdny)
 */
int ListFront(TList *list, TListData *data);

/**
 * @brief Vrati data z konce seznamu
 * @param list Ukazatel na seznam
 * @param data Ukazatel na strukturu TListData kam budou ulozena data
 * @return Hodnotu 1 pri uspechu, 0 pri chybe (zasobnik je prazdny)
 */
int ListBack(TList *list, TListData *data);


/**
 * @brief Vlozi data na dany index v seznamu
 * @note Pro vkladani lze pouzit index n oproti klasickemu n-1, prvek bude vlozen na konec seznamu
 * @param list Ukazatel na seznam
 * @param index Index, kam se data vlozi
 * @return Hodnotu 1 pri uspechu, 0 pri chybe
 */
int ListInsert(TList *list, int index, TListData data);

/**
 * @brief Odstrani data z daneho indexu v seznamu
 * @param list Ukazatel na seznam
 * @param index Index mazaneho prvku
 * @param data Volitelny parametr, ukazatel na strukturu TListData, kam budou vracena mazana data
 * @return Hodnotu 1 pri uspechu, 0 pri chybe
 */
int ListRemove(TList *list, int index, TListData *data);

/**
 * @brief Vrati data z daneho indexu v seznamu
 * @param list Ukazatel na seznam
 * @param index Index pozadovaneho prvku
 * @param data Ukazatel na strukturu TListData, kam budou vracena data
 * @return Hodnotu 1 pri uspechu, 0 pri chybe
 */
int ListGet(TList *list, int index, TListData *data);


/**
 * @brief Otestuje, jestli je zasobnik prazdny
 * @param list Ukazatel na seznam
 * @return Hodnotu 1 pokud je seznam prazdny, jinak 0
 */
int ListEmpty(TList *list);

/**
 * @brief Do hloubky porovna dva seznamy
 * @param list1 Ukazatel na prvni seznam
 * @param list2 Ukazatel na druhy seznam
 * @return 1 pri shode, jinak 0
 */
int ListCompare(TList *list1, TList *list2);

/**
 * @brief Porovna delky dvou seznamu
 * @param list1 Ukazatel na prvni seznam
 * @param list2 Ukazatel na druhy seznam
 * @return Hodnotu 0 pri schodne delce, -1 pokud je list1 kratsi, 1 pokud je list2 kratsi
 */
int ListCompareLength(TList *list1, TList *list2);

#endif

//Soubor src/list.h
