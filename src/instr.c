//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/instr.c                  //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////
#include "instr.h"
#include <stdio.h>
#include <stdlib.h>

void InstrExprPrint(TToken *tkn){
    if((tkn->type == TK_ID) || (tkn->type == TK_NUM_INTEGER) || (tkn->type == TK_NUM_DOUBLE) || (tkn->type == TK_NUM_STRING)){
        InstExprID(tkn);
    }
    else if((tkn->type == TK_PLUS) || (tkn->type == TK_MINUS) || (tkn->type == TK_MUL) || (tkn->type == TK_DIV) || (tkn->type == TK_MOD) || (tkn->type == TK_DIV_INT)){
        InstExprOperator(tkn);
    }
    else if((tkn->type == TK_INT2FLOAT) || (tkn->type == TK_FLOAT2INT)){
        InstExprConvert(tkn);
    }
    else if((tkn->type == TK_EQUAL) || (tkn->type == TK_NOT_EQUAL) || (tkn->type == TK_LESS) || (tkn->type == TK_GREATER) || (tkn->type == TK_LESS_EQUAL) || (tkn->type == TK_GREATER_EQUAL)){
        InstExprComparison(tkn);
    }
    else {
       CallError(ERR_INTERNAL);
    }
}

void InstExprID(TToken *tkn){
    if(tkn->type == TK_ID)  printf("PUSHS LF@p%%%s\n", tkn->string);
    if(tkn->type == TK_NUM_INTEGER) printf("PUSHS int@%s\n", tkn->string);
    if(tkn->type == TK_NUM_DOUBLE)  printf("PUSHS double@%s\n", tkn->string);
    if(tkn->type == TK_NUM_STRING)  printf("PUSHS string@%s\n", tkn->string);
}

void InstExprOperator(TToken *tkn){
    if(tkn->type == TK_PLUS)    printf("ADDS\n");
    if(tkn->type == TK_MINUS)   printf("SUBS\n");
    if(tkn->type == TK_MUL)     printf("MULS\n");
    if(tkn->type == TK_DIV)     printf("DIVS\n");
    //if(tkn->type == TK_MOD)     
    //if(tkn->type == TK_DIV_INT)
}

void InstExprConvert(TToken *tkn){
    //printf("Token: Type: %-22s String: %s\n", TokenTypes[tkn->type], tkn->string );
    if(tkn->type == TK_INT2FLOAT)   printf("INT2FLOATS\n");
    if(tkn->type == TK_FLOAT2INT)   printf("FLOAT2INTS\n");
}

void InstExprComparison(TToken *tkn){
    if(tkn->type == TK_EQUAL)   printf("EQS\n");
    //if(tkn->type == TK_NOT_EQUAL)   printf("NOT EQS\n");
    if(tkn->type == TK_LESS)    printf("LTS\n");
    if(tkn->type == TK_GREATER) printf("GTS\n");
    //if(tkn->type == TK_LESS_EQUAL)  printf("LTS OR EQS\n");
    //if(tkn->type == TK_GREATER_EQUAL)   printf("GTS OR EQS\n");
}