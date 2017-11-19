//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/parser2.h                //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#ifndef PARSER2_H
#define PARSER2_H

#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "symtable.h"

TToken* GetNextDestroyOldToken(TToken *tkn, int canGetEOL);
int SyntaxStartParse();

int Syntaxx_Program(TToken **tkn);
int Syntaxx_ListDecDef(TToken **tkn);
int Syntaxx_FunctionHead(TToken **tkn);
int Syntaxx_ListParam(TToken **tkn);
int Syntaxx_Param(TToken **tkn);
int Syntaxx_NextParam(TToken **tkn);
int Syntaxx_DataType(TToken **tkn);
int Syntaxx_FunctionEnd(TToken **tkn);
int Syntaxx_ScopeDef(TToken **tkn);
int Syntaxx_ScopeHead(TToken **tkn);
int Syntaxx_ScopeEnd(TToken **tkn);
int Syntaxx_ScopeAfter(TToken **tkn);
int Syntaxx_FunctionBody(TToken **tkn);
int Syntaxx_ListVarDef(TToken **tkn);
int Syntaxx_ListCommand(TToken **tkn);
int Syntaxx_Command(TToken **tkn);
int Syntaxx_ListExpression(TToken **tkn);
int Syntaxx_Condition(TToken **tkn);
int Syntaxx_RO(TToken **tkn);
int Syntaxx_Assignment(TToken **tkn);
int Syntaxx_ListInParam(TToken **tkn);
int Syntaxx_InParam(TToken **tkn);
int Syntaxx_NextInParam(TToken **tkn);
int Syntaxx_Term(TToken **tkn);

int Syntaxx_Expression(TToken **tkn);


#endif

// Soubor: src/parser2.h

