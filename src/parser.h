//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/parser.h                 //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include "token.h"

// TODO: Pokud vyjde cast, tak z globalnich promenych predelat na parametry funkci.

/**
 * @brief Uklada stav zda se bude cist novy znak ze vstupu nebo se bude pokracovat s jiz nactenym znakem.
 */
extern int repeatLastChar;
/**
 * @brief Ukozatel na prave nacitany token.
 */
extern TToken* readToken;
/**
 * @brief Prave nacteny znak, ktery se zpracovava.
 */
extern int readLastChar;
/**
 * @brief Cislo prave zpracovavaneho radku ze vstupu.
 */
extern unsigned long int readLineNumber;

//--------------------------

/**
 * @brief Rozhoduje zda znak c patri mezi znaky, ktere je dovoleno ignorovat.
 * @param  c Znak o kterem chceme vedet zda ho ignorovat.
 * @return Vraci 1 pokud muze byt znak ignorovan, jinak vraci 0.
 */
int CanBeIgnored(char c);

/**
 * @brief Nacita vstup, ktery prevede na token.
 * @return Ukazatel na nove nacteny token.
 */
TToken* GetNextToken();

//--------------------------

// Stavove funkce
// Jedna se o interni funkce, ktere zjednodusuji detekci typu tokenu.
void State_InlineComment();
void State_DivOrMultilineComment();
void State_ID();
void State_Number();
void State_ExMark();
void State_Equal();
void State_GreaterThan();
void State_LessThan();

void State_MultilineComment();
void State_Double();
void State_CanDoubleE();
void State_DoubleE();
void State_String();
void State_SpecialChar();


#endif

// Soubor: src/parser.h

