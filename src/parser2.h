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
#include "ast.h"

TToken* GetNextDestroyOldToken(TToken *tkn, int canGetEOL);
int SyntaxStartParse();

// nodeAST je aktualni uzel, ktery je potreba teprve vytvorit/inicializovat.
int Syntaxx_Program(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ListDecDef(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_FunctionHead(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, int isDeclareNow);
int Syntaxx_ListParam(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, int isDeclareNow, int checkParamNow);
int Syntaxx_Param(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, int isDeclareNow, int checkParamNow, int indexOfParam);
int Syntaxx_NextParam(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, int isDeclareNow, int checkParamNow, int indexOfParam);
int Syntaxx_DataType(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **elem);
int Syntaxx_FunctionEnd(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, int isDeclareNow);
int Syntaxx_ScopeDef(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ScopeHead(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel);
int Syntaxx_ScopeEnd(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_ScopeAfter(TToken **tkn, TATSNode **nodeAST);
int Syntaxx_FunctionBody(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel);
int Syntaxx_ListVarDef(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel);
int Syntaxx_VarDefAssigment(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, TTokenType tokenType);
int Syntaxx_ListCommand(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel);
int Syntaxx_Command(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel);
int Syntaxx_ListExpression(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel);
int Syntaxx_Condition(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel);
int Syntaxx_Assignment(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel);
int Syntaxx_ListInParam(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, symtable_elem_t **fel);
int Syntaxx_InParam(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, symtable_elem_t **fel, int indexOfParam);
int Syntaxx_NextInParam(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel, symtable_elem_t **fel, int indexOfParam);
int Syntaxx_Term(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel);

// nodeAST je nadrazeny uzel nikoliv uzel Expression.
// pr1: AST_ListExpression 
// pr2: AST_Condition
int Syntaxx_Expression(TToken **tkn, TATSNode **nodeAST, symtable_elem_t **gel);

int Semantic_ControlExpression( TList **listPostFix, TATSNode **nodeAST, symtable_elem_t **gel );

st_datatype_t TokenTypeToSymDataType(TTokenType type);
TTokenType SymDataTypeToTokenType(st_datatype_t type);

void SyntaxInitBuildInFunction();

#endif

// Soubor: src/parser2.h

