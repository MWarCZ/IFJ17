//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/symtable.c               //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dstring.h"
#include "symtable.h"
#include "error.h"

/*Inicializace tabulky*/
symtable_t *SymtableInit(){
    symtable_t *t = malloc(sizeof(symtable_t));
    t->array = malloc(sizeof(struct symtable_elem_t)*TABLE_SIZE);     //dynamicka alokace hashovaci tabulky
    if(t == NULL)  { return NULL; }
    t->size=TABLE_SIZE;                                               //inicializace arr_size                                                       //inicializace n
    for(unsigned int i=0;i<TABLE_SIZE;i++){                               //cyklus inicializace pole seznamů
        t->array[i]=NULL;
    }
    return t; 
}

/*Vložení prvku*/
symtable_elem_t *AddElemGlobal(symtable_t *globalTable, char *globalToken){

    unsigned int symtabKey = hash_function(globalToken);

    symtable_elem_t *add = (symtable_elem_t*) malloc(sizeof(symtable_elem_t));
    if(add == NULL){
        CallError(ERR_INTERNAL);
    }

    add->name = strdup(globalToken);
    add->nextElem = globalTable->array[symtabKey];
    globalTable->array[symtabKey] = add;
    /*bool declared TODO*/
    /*bool defined TODO*/
    /*local table TODO*/
    return add;
}

/*Zruší tabulku*/
void SymtableFree(symtable_t *globalTable){
    symtable_elem_t *delete;

    for(int i = 0;i<TABLE_SIZE;i++){
        while(globalTable->array[i]!=NULL){
            delete = globalTable->array[i];
            globalTable->array[i] = delete->nextElem;

            if(delete->local_symtable != NULL){
                SymtableFree(delete->local_symtable);
            }

            free(delete->name);
            free(delete);
        }
    }
}

/*Rozptylovací fce*/
unsigned int hash_function(const char *str){
    unsigned int h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++){
        h = 65599*h + *p;
    }
    return h;
}