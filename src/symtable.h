//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/symtable.h               //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "error.h"
#include "token.h"
#include "dstring.h"
#include "list.h"

#define TABLE_SIZE 100

/**
 * @brief Typ elementu
 * */
typedef enum{
    SYM_TYPE_VAR,
    SYM_TYPE_PARAM,
    SYM_TYPE_FUNCTION
} st_elemtype_t;

/**
 * @brief Typ dat elementu
 * */
typedef enum{
    SYM_DATATYPE_ERROR,
    SYM_DATATYPE_VOID,
    SYM_DATATYPE_INT,
    SYM_DATATYPE_DOUBLE,
    SYM_DATATYPE_STRING
} st_datatype_t;

/**
 * @brief hodnota elementu
 * */
typedef union{
    int iElemVal;
    double dElemVal;
    char *sElemVal;
} st_value_t;

typedef struct symtable_t symtable_t;

/**
 * @brief struktura elementu tabulky 
 * */
typedef struct symtable_elem_t{
    char *name;
    st_elemtype_t elemType;
    st_datatype_t dataType;
    st_value_t value;
    int declared; // deklarovano pomoci 'declare function xyz ...' - jen hlavicka
    //bool initialized; //MW 
    int defined; //MW // definovano pomoci 'function xyz ...' - hlavicka a telo funkce
    //TODO - doplňky pro element
    TList *listParam;//MW //List s parametry funkce. NULL=Funkce nebyla deklarovana ani definovana
    symtable_t *local_symtable;
    struct symtable_elem_t *nextElem;
} symtable_elem_t;

/**
 * @brief struktura tabulka
 * */
struct symtable_t{
    struct symtable_elem_t *array[TABLE_SIZE];
};

/**
 * @brief Rozptylovací funkce
 * @param pole charů
 * */
unsigned hash_function(const char *str);//

/**
 * @brief Inicializace Hash tabulky 
 * @param
 * */
void SymtableInit(symtable_t **globalTable);//

/**
 * @brief Vyhledá funkci
 * @param
 * */
symtable_elem_t *SymtableFind(symtable_t *globalTable, char *globalToken);//

/**
 * @brief Vyhledá ve funkci záznam o proměnné
 * @param
 * */
symtable_elem_t *SymtableFindLocal(symtable_t *globalTable, char *globalToken, char *localToken);//

/**
 * @brief Přidá element do globální tabulky
 * 
 * */
symtable_elem_t *AddElemGlobal(symtable_t *globalTable, char *globalToken);//

/**
 * @brief inicializace lokální tabulky
 * @param
 * */
symtable_elem_t *AddElemLocal(symtable_t *globalTable, char *globalToken, char *localToken);

/**
 * @brief Doplní hodnoty struktury elementu
 * 
 * */
symtable_elem_t *ElementBuiltIn(symtable_t *globalTable,char* Token /*todo*/);

/**
 * @brief Zruší tabulku 
 * @param
 * */
void SymtableFree(symtable_t *globalTable);//
#endif

//  src/symtable.h
