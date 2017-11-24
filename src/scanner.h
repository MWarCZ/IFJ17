//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/scanner.h                //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef SCANNER_H
#define SCANNER_H

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
 * @brief Uvolni z pameti interni promene vznikle pri generovani tokenu. Pouzit pred ukoncenim programu.
 */
void ClearScanner();

/**
 * @brief Nacita vstup, ktery prevede na token. 
 * Pokud je interne nastaveno (Pomoci funkce RepatLastToken() ) , ze se ma zopakovat vraceni naposled nacteneho tokenu.
 * tak nedojde k novemu cteni ale vrati se naposledy nacteny token.
 * @return Ukazatel na nove nacteny token.
 */
TToken* GetNextToken();

/**
 * @brief Nastavi interne, aby funkce GetNextToken() pri dalsim volani vratila stejny token jako pri predchozim volani.
 */
void RepeatLastToken();

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

/**
 * @brief Funkce umoznuje overit si zda scanner spravne funguje. Zpracovava stdin a na vystup tiskne informace o Tokenech nebo o chybe.
 * Staci funkci zavola v tele main.
 */
void TestScanner();

#endif

// Soubor: src/scanner.h

