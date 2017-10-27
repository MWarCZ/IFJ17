//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/error.h                  //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef ERROR_H
#define ERROR_H

typedef enum{
    ERR_OK = 0,             //Everything is fine ;)
    ERR_LEX = 1,            //error in lexical analyzer
    ERR_SYN = 2,            //error in syntactic analyzer
    ERR_SEM = 3,            //Semantics error for undefined function/variable
    ERR_COMP = 4,           //Semantics error for uncompatible aritmetics, strings, wrong function arguments
    ERR_SEM_OTHER = 6,      //other semantics error
    ERR_INTERNAL = 99      //internal error 
}TError;

extern TError ERR_EXIT_STATUS;

void DefError(TError err);
void CallError(TError err);

#endif

//Soubor: src/error.h
