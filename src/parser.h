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


extern int repeatLastChar;
extern TToken* readToken;
extern int readLastChar;
extern unsigned long int readLineNumber;

TToken* GetNextToken();
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

