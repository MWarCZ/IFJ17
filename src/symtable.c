//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/symtable.c               //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#include "symtable.h"
/*Rozptylovací fce*/
unsigned int hash_function(char *str){
    unsigned int h=0;
    unsigned char *p;
    for(p=(unsigned char*)str; *p!='\0'; p++){
        h = 65599*h + *p;
    }
    return h;
}
/*Inicializace a vytvoření tabulky*/
htab_t *htab_init(unsigned size){
    htab_t *t = malloc(sizeof(htab_t));
    t->ptr = malloc(sizeof(struct htab_listItem)*size);     //dynamicka alokace hashovaci tabulky
    if(t == NULL){
        return NULL;
    }
    t->arr_size=size;                                       //inicializace arr_size
    t->n=0;                                                 //inicializace n
    for(unsigned int i=0;i<size;i++){                       //cyklus inicializace pole seznamů
        t->ptr[i]=NULL;
    }
    return t;      
}
/*Hledání v tabulce*/
htab_listItem *htab_find(htab_t *t, char *key){
    
}
/*Vytvoří prvek seznamu*/
htab_listItem *htab_create(htab_t *t, char *key){

}
/*Provedení funkce pro každý prvek*/
void htab_foreach(htab_t *t, void (*function)(char *key, ??,htab_listItem *item)){

}
/*Odstranení prvku*/
bool htab_remove(htab_t *t,const char *key){

}
/*Zrušení všech prvků*/
void htab_clear(htab_t *t){

}
/*Zrušení celé tabulky*/
void htab_free(htab_t *t){

}