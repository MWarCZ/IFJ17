//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/error.c                  //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef ERROR_C
#define ERROR_C

#include <stdio.h>
#include <stdlib.h>
#include "error.h"

TError ERR_EXIT_STATUS = ERR_OK;
/*Function sets the right error message*/
void DefError(TError err){
    switch(err){
        case ERR_OK:
            break;
        case ERR_LEX:
            fprintf(stderr,"Lexical error\n");
            break;
        case ERR_SYN:
            fprintf(stderr,"Syntactic error\n");
            break;
        case ERR_SEM:
            fprintf(stderr,"Semantics error for undefined function/variable\n");
            break;
        case ERR_COMP:
            fprintf(stderr,"Semantics error for uncompatible aritmetics, strings, wrong function arguments\n");
            break;
        case ERR_SEM_OTHER:
            fprintf(stderr,"other semantics error\n");
            break;
        case ERR_INTERNAL:
            fprintf(stderr,"Internal error\n");
            break;
    }   
}
/*Error function for handling warning or exit error*/
void CallError(TError err){
    ERR_EXIT_STATUS = err;
    DefError(err);
    if(err == ERR_INTERNAL){
        exit(1);
    }
}

#endif

// Soubor: src/error.c
