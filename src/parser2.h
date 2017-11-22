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

int Syntaxx_Program(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ListDecDef(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_FunctionHead(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ListParam(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_Param(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_NextParam(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_DataType(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_FunctionEnd(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ScopeDef(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ScopeHead(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ScopeEnd(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ScopeAfter(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_FunctionBody(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ListVarDef(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_VarDefAssigment(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ListCommand(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_Command(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ListExpression(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_Condition(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_Assignment(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ListInParam(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_InParam(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_NextInParam(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_Term(TToken **tkn, TATSNode **nodeAST);

int Syntaxx_Expression(TToken **tkn);

#endif

// Soubor: src/parser2.h

