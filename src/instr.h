//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/instr.h                  //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef INSTR_H
#define INSTR_H

#include "error.h"
#include "token.h"

/**
* @brief Tisk triadresneho kodu vyrazu
* @param tkn Token
**/
void InstrExprPrint(TToken *tkn);
/**
* @brief Tisk operandu
* @param tkn Token
**/
void InstExprID(TToken *tkn);
/**
* @brief Tisk operatoru
* @param tkn Token
**/
void InstExprOperator();
/**
* @brief Tisk konverze datoveho typu
* @param tkn Token
**/
void InstExprConvert(TToken *tkn);
/**
* @brief Tisk porovnavaciho operatoru
* @param tkn Token
**/
void InstExprComparison(TToken *tkn);

#endif

//Soubor src/instr.h