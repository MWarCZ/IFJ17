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

/*Vlastní strdup*/
char *strdup (const char *s) {
    char *d = malloc (strlen (s) + 1);   
    if (d == NULL) return NULL;          
    strcpy (d,s);                        
    return d;                            
}

/*Inicializace tabulky*/
void SymtableInit(symtable_t **t){
    *t = (symtable_t*) malloc(sizeof(symtable_elem_t)*TABLE_SIZE);
    if(*t == NULL){
        CallError(ERR_INTERNAL);
    }
    for(int i = 0;i < TABLE_SIZE; i++){
        (*t)->array[i] = NULL;
    }
}

/*Vložení prvku*/
symtable_elem_t *AddElemGlobal(symtable_t *globalTable, char *globalToken){

    unsigned int symtabKey = hash_function(globalToken);

    symtable_elem_t *add = (symtable_elem_t*) malloc(sizeof(symtable_elem_t));
    if(add == NULL){
        CallError(ERR_INTERNAL);
    }

    add->name = strdup(globalToken);    //nazev tokenu
    add->nextElem = globalTable->array[symtabKey];  //posune seznam
    globalTable->array[symtabKey] = add;    //dosadí na zacatek novy element
    add->declared = false; //MW
    add->defined = false; //MW
    //add->local_symtable = NULL; //MW
    SymtableInit( &(add->local_symtable) );//MW
    add->listParam = NULL;//MW
    return add; //vraci nove vytvoreny element
}

/*Vyhledá funkci v globální tabulce*/
symtable_elem_t *SymtableFind(symtable_t *globalTable, char *globalToken){
    if(globalTable == NULL){
        return NULL;
    }
    
    unsigned int globalKey = hash_function(globalToken);

    symtable_elem_t *tmp = globalTable->array[globalKey];

    while(tmp != NULL){
        if(strcmp(tmp->name,globalToken) == 0){
            return tmp;
        }
        tmp = tmp->nextElem;
    }
    return NULL;
}

/*Vyhledá položku v lokální tabulce*/
symtable_elem_t *SymtableFindLocal(symtable_t *globalTable, char *globalToken, char *localToken){
    symtable_elem_t *tmp = SymtableFind(globalTable,globalToken);
    if(tmp == NULL){
        return NULL;
    }
    else{
        if(tmp->local_symtable == NULL){
            return NULL;
        }

        unsigned int localKey = hash_function(localToken);

        symtable_elem_t *localTmp = tmp->local_symtable->array[localKey];

        while(localTmp != NULL){
            if(strcmp(localTmp->name,localToken) == 0){
                return localTmp;
            }
            localTmp = localTmp->nextElem;
        }
        return NULL;
    }

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
            if(delete->listParam != NULL) {//MW
              ListClear(delete->listParam);//MW
              free(delete->listParam);//MW
            }//MW

            free(delete->name);
            free(delete);
        }
    }
    free(globalTable);//MW
}

/*Rozptylovací fce*/
unsigned int hash_function(const char *str){
    unsigned int h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++){
        h = 65599*h + *p;
    }
    return (h % TABLE_SIZE);
}
